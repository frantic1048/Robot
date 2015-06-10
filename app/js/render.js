
var ipc = require('ipc');
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
var Roboto = function () {
  var canvas = null,  // Canvas Element to Draw
      context = null, // Canvas Context
      lastFrameTime = performance.now(); // time of last frame rendered(millisecond)
      FPS = 24, // max FPS of rendering
      running = false, // running status

      // origin point for drawing
      baseX = 0,
      baseY = 0;

  function rX (x) { return baseX + x; }
  function rY (y) { return baseY + y; }
  function draw () {
    var now = performance.now();
    if (running && (now - lastFrameTime >= 1000/FPS)) {
      context.fillStyle = "rgb(170,66,255)";
      context.fillRect (rX(10),rY(10), 55, 50);

      context.fillStyle = "rgba(72, 210, 255, 0.5)";
      context.fillRect (rX(30),rY(30), 55, 50);
    }
    requestAnimationFrame(draw);
  }
  return {
    stop : function () {
      running = false;
    },
    start : function () {
      running = true;
      this.resize();
      draw();
    },
    init : function () {
      canvas = document.getElementById("canvas");
      context = canvas.getContext('2d');
      this.resize();
      this.new();
      this.start();

      window.addEventListener('resize',this.resize);
    },
    resize : function () {
      canvas.setAttribute("width",window.innerWidth - 16 * 12);// 16px * 12rem is sidebar's width
      canvas.setAttribute("height",window.innerHeight);
      baseX = canvas.getAttribute("width") * 0.5;
      baseY = canvas.getAttribute("height") * 0.5;
      console.log("width: " + window.innerWidth + "  height: " + window.innerHeight);
    },
    new : function () {
      //...
      this.start();
    },
    setFPS : function (fps) {
      var nfps = parseInt(fps,10) || 24;
      if (nfps > 0) FPS = nfps;
    }
  };
};
