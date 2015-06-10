
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
      FPS = 5, // max FPS of rendering
      running = false, // running status

      // origin point for drawing
      baseX = 0,
      baseY = 0;

  function r (){ return Math.random()*255 };
  function ir () {return parseInt(r(),10)};

  function rX (x) { return baseX + x; }
  function rY (y) { return baseY + y; }

  function draw () {

    var now = performance.now();
    if (running && (now - lastFrameTime >= 1000/FPS)) {
      lastFrameTime = now;

      context.fillStyle = "rgb(" + ir() + "," + ir() + "," + ir() + ")";
      context.fillRect (rX(r()),rY(r()), r(), r());

      context.fillStyle = "rgb(" + ir() + "," + ir() + "," + ir() + ")";
      context.fillRect (rX(r()),rY(r()), r(), r());
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
      console.log("window> width: " + (window.innerWidth - 16 * 12) + "  height: " + window.innerHeight);
      console.log("canvas> width: " + canvas.getAttribute("width") + "  height: " + canvas.getAttribute("height"));
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
