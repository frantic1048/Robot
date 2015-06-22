var ipc = require('ipc'),

    // Emscripten bindings
    Point = Module.Point,
    Vector = Module.Vector,
    Bots = Module.Bots;

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
  var layout = null,  // reference for layout
      canvas = null,  // Canvas Element to Draw
      context = null, // Canvas Context
      lastFrameTime = performance.now(); // time of last frame rendered(millisecond)
      FPS = 60, // max FPS of rendering
      running = false, // running status

      // origin point for drawing
      baseX = 0,
      baseY = 0;

  function rX (x) { return baseX + x; }
  function rY (y) { return baseY + y; }

  function draw () {

    var now = performance.now();
    if (running && (now - lastFrameTime >= 1000/FPS)) {
      lastFrameTime = now;

      context.clearRect(0
                       ,0
                       ,canvas.getAttribute("width")
                       ,canvas.getAttribute("height")); // clear previous frame

      context.fillStyle = "rgb(112,169,255)";
      context.fillRect (rX(-10),rY(-10),20, 20);

      context.fillStyle = "rgb(204,108,255)";
      context.fillRect (rX(0),rY(0),30,40);
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
      layout = document.getElementById("layout");
      this.resize();
      this.new();
      this.start();
      window.addEventListener('resize',this.resize);
    },
    resize : function () {
      if (canvas) canvas.remove();
      canvas = layout.insertBefore(document.createElement("canvas"),layout.firstChild);
      canvas.setAttribute('id','canvas');
      context = canvas.getContext('2d');

      canvas.setAttribute("width",window.innerWidth - 16 * 12);// 16px * 12rem is sidebar's width
      canvas.setAttribute("height",window.innerHeight);
      baseX = canvas.getAttribute("width") * 0.5;
      baseY = canvas.getAttribute("height") * 0.5;
    },
    new : function () {
      //...
      this.resize();
      this.start();
    },
    setFPS : function (fps) {
      var nfps = parseInt(fps,10) || 24;
      if (nfps > 0) FPS = nfps;
    }
  };
};
