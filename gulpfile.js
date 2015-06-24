var gulp  = require('gulp'),
    shell = require('gulp-shell');


gulp.task('default',function(){
  return gulp.src("src/Robot.cpp")
    .pipe(shell([
      // -s : total memory
      //        default 16777216 = 16*1024*1024 Byte = 16 MiB
      //        using   134217728 Byte = 128 MiB
      // -O3: enable on production
      'em++ --bind -v <%= file.path %> -s TOTAL_MEMORY=134217728 --memory-init-file 0 -O3 -o app/js/Robot.js'
    ]));
});


gulp.task('stat',shell.task([
    'wc --lines\
     gulpfile.js \
     package.json \
     app/package.json \
     app/main.js \
     app/index.html \
     app/js/render.js \
     app/style/custom.css \
     src/*.cpp'
]));
