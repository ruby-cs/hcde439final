let port;
let writer;

let level = 1;
// 20s per level
let timeLeft = 20.0;
let timerInterval = null;

let lastHitTime = 0; // Debounce timer for "caught" detection

const timerDisplay = document.getElementById("timer");
const levelDisplay = document.getElementById("level");
const connectBtn = document.getElementById("connectBtn");
const startBtn = document.getElementById("startBtn");

// Connecting Arduino
connectBtn.addEventListener("click", async () => {
  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 115200 });

    writer = port.writable.getWriter();

    readLoop();

    console.log("Connected!");
    connectBtn.innerText = "Connected!";
    connectBtn.disabled = true;

  } catch (err) {
    console.error("Connection error:", err);
  }
});

// Reading from Arduino
async function readLoop() {
  const decoder = new TextDecoderStream();
  const readableStreamClosed = port.readable.pipeTo(decoder.writable);
  const inputStream = decoder.readable;
  const reader = inputStream.getReader();

  while (true) {
    const { value, done } = await reader.read();
    if (done) break;
    if (!value) continue;

    if (value.includes("caught")) {
      const now = Date.now();
      if (now - lastHitTime > 1000) {
        nextLevel();
        lastHitTime = now;
      }
    }
  }
}

// Start game
startBtn.addEventListener("click", () => {
  resetGame();
  startTimer();
  sendSpeed();
});

// Level increase
function nextLevel() {
  level++;
  levelDisplay.textContent = level;
  // Timer resets every level
  resetTimer();
  sendSpeed();
}

// Send level to Arduino
function sendSpeed() {
  if (!writer) return;
  writer.write(new TextEncoder().encode(level + "\n"));
}

// Timer
function resetTimer() {
  clearInterval(timerInterval);
  timeLeft = 20.0;
  timerDisplay.textContent = timeLeft.toFixed(1);

  timerInterval = setInterval(() => {
    timeLeft -= 0.1;
    if (timeLeft <= 0) {
      clearInterval(timerInterval);
      alert("Time's up! Game over");
    }
    timerDisplay.textContent = Math.max(0, timeLeft).toFixed(1);
  }, 100);
}

function startTimer() {
  resetTimer();
}

// Reset game
function resetGame() {
  level = 1;
  levelDisplay.textContent = level;

  clearInterval(timerInterval);
  timeLeft = 20.0;
  timerDisplay.textContent = timeLeft.toFixed(1);
}