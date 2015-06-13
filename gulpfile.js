var gulp  = require('gulp'),
    shell = require('gulp-shell');


gulp.task('default',function(){
  return gulp.src("src/roboto.cpp")
    .pipe(shell([
      'em++ --bind -v <%= file.path %> -o app/js/roboto.js'
    ]))
});
