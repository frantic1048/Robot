var ipc = require('ipc'),

    // Emscripten bindings
    Simulator = Module.Simulator;

// window controls
function Wminimize() {
  ipc.send('Wminimize');
}
function Wmaximize() {
  ipc.send('Wmaximize');
}
function Wclose() {
  ipc.send('Wclose');
}

// robot simulator
var Robot = function () {
  var simulationArea = null,  // reference for layout
      canvas = null,  // Canvas Element to Draw
      context = null, // Canvas Context
      lastFrameTime = performance.now(); // time of last frame rendered(millisecond)
      FPS = 60, // max FPS of rendering
      running = false, // running status

      // origin point for drawing
      baseX = 0,
      baseY = 0;

      simulation = null;

  function rX (x) { return baseX + x; }
  function rY (y) { return baseY + y; }

  function draw () {

    var now = performance.now();
    if (running && (now - lastFrameTime >= 1000/FPS)) {
      lastFrameTime = now;

      // clear previous frame
      context.clearRect(0
                       ,0
                       ,canvas.getAttribute("width")
                       ,canvas.getAttribute("height"));

      // draw robots
      var bots = simulation.currentBots;
      var bot = null;
      var botPath = null;
      var botVisionRaduis = bots.visionRaduis;
      var botBodyRadius = botVisionRaduis / 10;
      var eachBot = function (operation) {
        while(bots.indexOfBotIterator() !== -1){
            bot = bots.nextBot();
            operation(bot);
        }
      };

      // vision circle
      context.save();
      context.setLineDash([5, 2]);
      context.lineDashOffset = now/30;
      context.strokeStyle = "rgba(102, 202, 255, 0.4)";
      eachBot(function(bot){
        context.beginPath();
        context.arc(rX(bot.x),rY(bot.y),botVisionRaduis,0,2*Math.PI,true);
        context.stroke();
      });

      // direction line
      context.restore();
      context.save();
      context.strokeStyle = "rgba(45, 252, 255, 0.2)";
      eachBot(function(bot){
        context.beginPath();
        context.moveTo(rX(bot.x),rY(bot.y));
        context.lineTo(rX(bot.x + botVisionRaduis*Math.cos(bot.theta)),rY(bot.y + botVisionRaduis*Math.sin(bot.theta)));
        context.stroke();
      });

      // bot body
      context.restore();
      context.save();
      context.fillStyle = "rgba(0,157,255,1)";
      eachBot(function(bot){
        context.beginPath();
        context.arc(rX(bot.x),rY(bot.y),botBodyRadius,0,2*Math.PI,true);
        context.fill();
      });

      // compute next tick's status
      simulation.nextTick();
    }
    requestAnimationFrame(draw);
  }
  return {
    stop : function () {
      running = false;
    },
    start : function () {
      running = true;
      draw();
    },
    init : function () {
      simulationArea = document.getElementById("simulation-area");
      this.resize();
      this.new();
      this.start();
      window.addEventListener('resize',this.resize);
    },
    resize : function () {
      if (canvas) canvas.remove();
      var rect = simulationArea.getBoundingClientRect();

      canvas = simulationArea.appendChild(document.createElement("canvas"));
      canvas.setAttribute('id','canvas');
      context = canvas.getContext('2d');
      canvas.setAttribute("width",rect.width);
      canvas.setAttribute("height",rect.height);

      rect = canvas.getBoundingClientRect();
      baseX = rect.width * 0.5;
      baseY = rect.height * 0.5;
    },
    new : function () {
      if (simulation !== null) simulation.delete();
      simulation = new Simulator(20,20);
      this.resize();
      this.start();
    },
    setFPS : function (fps) {
      var nfps = parseInt(fps,10) || 24;
      if (nfps > 0) FPS = nfps;
    }
  };
};
