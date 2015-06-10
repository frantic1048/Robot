var app = require('app');  // Module to control application life.
var BrowserWindow = require('browser-window');  // Module to create native browser window.
var ipc = require('ipc'); // for window message communication

// Report crashes to our server.
require('crash-reporter').start();

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the javascript object is GCed.
var mainWindow = null;

// Quit when all windows are closed.
app.on('window-all-closed', function() {
  if (process.platform != 'darwin')
    app.quit();
});

// This method will be called when atom-shell has done everything
// initialization and ready for creating browser windows.
app.on('ready', function() {
  // Create the browser window.
  var robotoWindow = {
      width:800,
      height:600,
      "min-width": 800,
      "min-height": 600,
      frame:false,
      show:false
  };
  mainWindow = new BrowserWindow(robotoWindow);
  mainWindow.setSize(800,601);

  // disable default electron menubar
  // mainWindow.setMenu(null);

  // and load the index.html of the app.
  mainWindow.loadUrl('file://' + __dirname + '/index.html');

  mainWindow.show();

  // fix the weird canvas sizeing
  setTimeout(function(){mainWindow.setSize(800,600);},500);

  // Emitted when the window is closed.
  mainWindow.on('closed', function() {
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow = null;
  });


  ipc.on('Wminimize', function(event, arg) {
    mainWindow.minimize();
  });
  ipc.on('Wmaximize', function(event, arg) {
    if (mainWindow.isMaximized()){
      mainWindow.unmaximize();
    } else {
      mainWindow.maximize();
    }
  });
  ipc.on('Wclose', function(event, arg) {
    mainWindow.close();
  });

});
