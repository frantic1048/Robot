var gulp  = require('gulp'),
    shell = require('gulp-shell');


gulp.task('default',function(){
  return gulp.src("src/Robot.cpp")
    .pipe(shell([
      // -s: max memory
      // default 16777216 = 16*1024*1024 Byte = 16 MiB
      // using   134217728 = 128 MiB
      'em++ --bind -v <%= file.path %> -s TOTAL_MEMORY=134217728 -O3 --memory-init-file 0 -o app/js/Robot.js'
    ]))
});
