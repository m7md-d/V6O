const $ = (id) => document.getElementById(id);

function formatTime(seconds) {
  const m = Math.floor(seconds / 60);
  const s = String(Math.floor(seconds % 60)).padStart(2, "0");
  return `${m}:${s}`;
}
const pours = [{ duration: 15, wait: 10 }];

let timer = null;
let run = null;

function buildSchedule() {
  const segments = [];
  let time = 0;

  pours.forEach((pour, index) => {
    if (index > 0 && pour.wait > 0) {
      segments.push({ type: "wait", index, start: time, length: pour.wait });
      time += pour.wait;
    }
    segments.push({ type: "pour", index, start: time, length: pour.duration });
    time += pour.duration;
  });

  return { segments, total: time };
}

function renderSteps() {
  const { segments, total } = buildSchedule();

  $("steps").innerHTML = segments
    .map((seg, i) => {
      const label = seg.type === "pour" ? `Pour ${seg.index + 1}` : "Wait";
      return `<li id="sg${i}"><span>${label}</span><span>${seg.length} s</span></li>`;
    })
    .join("");

  $("sTotal").textContent = formatTime(total);
  if (!run) $("sPour").textContent = `0/${pours.length}`;
}

function renderPours() {
  const disabled = run ? "disabled" : "";
  $("del").disabled = Boolean(run) || pours.length < 2;

  const header = `
    <div class="rw hdr">
      <span></span>
      <span>Wait before (s)</span>
      <span>Pour duration (s)</span>
    </div>`;

  const rows = pours
    .map((pour, i) => {
      const waitCell =
        i > 0
          ? `<input type="number" inputmode="numeric" min="0" max="60"
               value="${pour.wait}" data-i="${i}" data-f="wait"
               aria-label="Wait before pour ${i + 1}" ${disabled}>`
          : `<span class="na">—</span>`;

      return `
        <div class="rw${run ? " lock" : ""}">
          <b>${i + 1}</b>
          ${waitCell}
          <input type="number" inputmode="numeric" min="5" max="60"
            value="${pour.duration}" data-i="${i}" data-f="duration"
            aria-label="Duration of pour ${i + 1}" ${disabled}>
        </div>`;
    })
    .join("");

  $("cards").innerHTML = header + rows;
  renderSteps();
}

$("cards").oninput = (e) => {
  const input = e.target;
  if (run || !input.dataset.f || input.value === "") return;

  const field = input.dataset.f;
  const min = field === "duration" ? 5 : 0;
  const value = Math.min(60, Math.max(min, Number(input.value)));

  pours[Number(input.dataset.i)][field] = value;
  renderSteps();
};

$("cards").onchange = (e) => {
  const input = e.target;
  if (!input.dataset.f) return;

  const pour = pours[Number(input.dataset.i)];
  const field = input.dataset.f;

  if (input.value === "") pour[field] = field === "duration" ? 30 : 0;
  input.value = pour[field];
  renderSteps();
};

$("plus").onclick = () => {
  if (pours.length >= 12) return;

  pours.push({ duration: 15, wait: 5 });
  renderPours();

  $("cards").scrollTop = $("cards").scrollHeight;
};

$("del").onclick = () => {
  if (run || pours.length < 2) return;
  pours.pop();
  renderPours();
};

function resetBrew(message) {
  clearInterval(timer);
  run = null;

  $("go").disabled = false;
  $("phase").textContent = message || "Ready";
  $("stream").setAttribute("height", 0);
  $("arm").style.transform = "";

  renderPours();
}

$("go").onclick = () => {
  if (run) return;

  run = { time: 0, current: 1 };
  $("go").disabled = true;
  renderPours();

  // تفريغ الكوب
  $("cup").setAttribute("height", 0);
  $("cup").setAttribute("y", 273);

  let lastTick = performance.now();

  timer = setInterval(() => {
    const now = performance.now();
    run.time += (now - lastTick) / 1000;
    lastTick = now;

    const { segments, total } = buildSchedule();
    const totalPourTime = pours.reduce((sum, p) => sum + p.duration, 0);

    if (run.time >= total) {
      $("prog").style.width = "100%";
      $("cup").setAttribute("height", 62);
      $("cup").setAttribute("y", 211);
      $("sTime").textContent = formatTime(total);

      resetBrew("Your coffee is ready");
      document.querySelectorAll("#steps li").forEach((li) => li.classList.add("done"));
      return;
    }


    let current = segments.findIndex(
      (seg) => run.time >= seg.start && run.time < seg.start + seg.length
    );
    if (current < 0) current = segments.length - 1;

    const seg = segments[current];
    const progress = (run.time - seg.start) / seg.length;
    const isPouring = seg.type === "pour";

    run.current = isPouring ? seg.index + 1 : seg.index;

    let pouredTime = 0;
    segments.forEach((s, i) => {
      if (s.type !== "pour") return;
      if (i < current) pouredTime += s.length;
      else if (i === current) pouredTime += s.length * progress;
    });

    const swing = Math.sin(run.time * 0.5) * 40 * (1 - progress * 0.5);
    $("arm").style.transform = isPouring ? `translateX(${swing}px)` : "";
    $("stream").setAttribute("height", isPouring ? 26 : 0);

    $("sPour").textContent = `${run.current}/${pours.length}`;
    $("sTime").textContent = formatTime(run.time);
    $("sTotal").textContent = formatTime(total);
    $("prog").style.width = `${(run.time / total) * 100}%`;


    const cupHeight = (pouredTime / totalPourTime) * 62;
    $("cup").setAttribute("height", cupHeight);
    $("cup").setAttribute("y", 273 - cupHeight);

    segments.forEach((_, i) => {
      const li = $(`sg${i}`);
      if (!li) return;
      li.className = i < current ? "done" : i === current ? "on" : "";
    });

    $("phase").textContent = isPouring
      ? `Pour ${seg.index + 1} – Pouring`
      : "Waiting for the next pour";
  }, 50);
};
renderPours();
