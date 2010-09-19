#/bin/bash

# Simple shell script that uses imagemagick to grab snapshots of our example programs.
IMAGES="doc/site/claro/src/images/screenshots/gtk"

function grab_window() {
        windowid=$1
        file=$2

        rm -f "$file"
        sleep 2
        import -window "$windowid" -frame "$IMAGES/$file"
        kill $!
        sleep 1
}


function snap() {
        program=$1
        windowid=$2
        file=$3
        
        $program &
        grab_window "$windowid" "$file"
}


function src2html() {
        dest="$IMAGES/../examples"
        src=$1
        
        highlight -l $src -O $dest
}

snap build/canvas-test 'Canvas Clock' canvas-test.png
src2html examples/canvas/canvas-test.c

snap build/hello 'Hello, World!' hello.png
src2html examples/helloworld/hello.c

snap build/splitter-test 'Splitter Demo' splitter-test.png
src2html examples/splitter/splitter-test.c

#layout tests for fun
build/layout_any 400 400 50 30 "[menu|menu|menu|menu][b1<|b2<|b3<|b4<|b5<|b6<][_text][text.input|>send]" &
grab_window 'Layout Anything' layout_any.png
src2html examples/layout/layout_any.c

build/layout_test "[_textbox][input|send(60)]" &
grab_window 'My Simple Application' layout_test.png
src2html examples/layout/layout_test.c

build/layout_any 400 400 50 30 "[h1|h2|h3|h4|h5][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4][t1|t2|t3|t4|t4]" &
grab_window 'Layout Anything' simple_table.png

build/cldlg "[T.text][_][Cancel||Yes|No]" "Are you sure?" &
grab_window 'Dialog' cldlg.png
