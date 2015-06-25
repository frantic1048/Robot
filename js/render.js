var Simulator = Module.Simulator;// Emscripten binding

// robot simulator
var Robot = function () {
  var FPS = 30, // FPS limit of rendering
      robotCount = 40,
      mapVertexCount = 17,

      simulationArea = null,  // reference DOM element for simulation layout
      canvas = null,  // canvas element to draw simulation
      context = null, // canvas context
      simulation = null, // object handling a simulator instance

      lastFrameTime = performance.now(), // time of last frame rendered(millisecond)
      running = false, // running state
      drawing = false, // drawing state

      // origin point for drawing
      baseX = 0,
      baseY = 0;

  function rX (x) { return baseX + x; }
  function rY (y) { return baseY + y; }

  function draw () {
    if (drawing){ return; }
    else { drawing = true; }

    var now = performance.now();
    if (now - lastFrameTime >= 1000/FPS) {
      lastFrameTime = now;

      // clear previous frame
      context.clearRect(0
                       ,0
                       ,canvas.getAttribute("width")
                       ,canvas.getAttribute("height"));
      context.save(); // save default drawing state

      // draw map
      var map = simulation.currentMap;
      var vertexRadius = 3;
      var eachVertex = function (operation) {
        while(map.indexOfVertexIterator() !== -1){
            vertex = map.nextVertex();
            operation(vertex);
        }
      };

      var vertices = [];
      eachVertex(function(vertex){
        vertices.push(vertex);
      });

      // draw map:edge line
      context.restore();
      context.save();
      context.strokeStyle = "rgb(255, 185, 185)";
      context.beginPath();
      context.moveTo(rX(vertices[0].x),rY(vertices[0].y));
      vertices.slice(1).forEach(function(vertex){
        context.lineTo(rX(vertex.x), rY(vertex.y));
      });
      context.lineTo(rX(vertices[0].x),rY(vertices[0].y));
      context.stroke();

      // draw map:vertex
      context.restore();
      context.save();
      context.fillStyle = "rgba(255, 40, 40,0.8)";
      vertices.forEach(function(vertex){
        context.beginPath();
        context.arc(rX(vertex.x),rY(vertex.y),vertexRadius,0,2*Math.PI,true);
        context.fill();
      });

      // draw robots
      context.restore();
      context.save();
      var bots = simulation.currentBots;
      var bot = null;
      var botVisionRaduis = bots.visionRaduis;
      var botBodyRadius = 2;
      var eachBot = function (operation) {
        while(bots.indexOfBotIterator() !== -1){
            bot = bots.nextBot();
            operation(bot);
        }
      };

      // draw robots:vision circle
      context.restore();
      context.save();
      context.setLineDash([5,3]);
      context.lineDashOffset = now/30;
      context.strokeStyle = "rgba(102, 202, 255, 0.6)";
      context.lineWidth = "2";
      eachBot(function(bot){
        context.beginPath();
        context.arc(rX(bot.x),rY(bot.y),botVisionRaduis,0,2*Math.PI,true);
        context.stroke();
      });

      // draw robots:direction line
      context.restore();
      context.save();
      context.strokeStyle = "rgba(45, 252, 255, 0.4)";
      eachBot(function(bot){
        context.beginPath();
        context.moveTo(rX(bot.x),rY(bot.y));
        context.lineTo(rX(bot.x + botVisionRaduis*Math.cos(bot.theta)),rY(bot.y + botVisionRaduis*Math.sin(bot.theta)));
        context.stroke();
      });

      // draw robots:bot body
      context.restore();
      context.save();
      // context.fillStyle = "rgba(0,157,255,1)";
      context.fillStyle = "rgba(45, 252, 255, 1)";
      eachBot(function(bot){
        context.beginPath();
        context.arc(rX(bot.x),rY(bot.y),botBodyRadius,0,2*Math.PI,true);
        context.fill();
      });

      // compute next tick's status
      if (running) simulation.nextTick();
    }
    drawing = false;
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

      if (simulation !== null) draw();
    },
    new : function () {
      if (simulation !== null) simulation.delete();
      simulation = new Simulator(robotCount,mapVertexCount);
      this.resize();
      this.start();

      // light on state-running button
      document.querySelector('#state2+label').click();
    },
    setFPS : function (_fps) {
      var nfps = parseInt(_fps,10) || 24;
      if (nfps > 0) FPS = nfps;
    },
    setRobotCount : function (_robotCount) {
      var nRobotCount = parseInt(_robotCount,10) || 30;
      if (nRobotCount > 0){
         robotCount = nRobotCount;
         this.new();
       }
    },
    setVertexCount : function (_vertexCount) {
      var nVertexCount = parseInt(_vertexCount, 10) || 20;
      if (nVertexCount > 0){
        mapVertexCount = nVertexCount;
        this.new();
      }
    }
  };
};
