\page exemple Exemple

@htmlonly
<canvas class="emscripten" id="canvas" oncontextmenu="event.preventDefault()" tabindex="-1"></canvas>

<p id="output" />
<script>
    var Module = {
        print: (function() {
            var element = document.getElementById('output');
            return function(text) {
                element.innerHTML += text + "<br>";
            };
        })(),
        printErr: function(text) {
                if (arguments.length > 1) text = Array.prototype.slice.call(arguments).join(' ');
                if (0) { dump(text + '\n'); }
        },				
        canvas: (function() {
            var canvas = document.getElementById('canvas');
            return canvas;
        })()
    };
</script>
<script src="Simulatio_Renderer.js"></script>
@endhtmlonly
