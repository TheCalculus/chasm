# chasm
C(++) to HTML assembler (chasm) for webdev in C(++)<br>
templating engine keywords:<br>
<code>
    C_COMMENT, // c comments in chasm templator
    C_VARIABLE, // c variables accessed from chasm templator
    C_BLOCK, // c code blocks (ie loops) accessed from chasm templator
    HTML_ATTRIBUTE, // html attributes (ie href) 
    HTML_CLOSE_TAG, // right angle bracket (>)
    HTML_COMMENT, // html comment blocks <!-- ... -->
    HTML_OPEN_TAG, // left angle bracket (<)
    JS_COMMENT, // javascript comments in chasm templator
    JS_VARIABLE, // javascript variables accessed in chasm templator
    JS_BLOCK, // javascript blocks in chasm templator
    TEMPLATE_CLOSE_EXPRESSION, // template expression close (}})
    TEMPLATE_OPEN_EXPRESSION, // template expression open ({{)
</code><br>
TODO:
<ul>
    <li>finish readme</li>
    <li>create core functionality</li>
    <li>remove unnecessary webserver.c from project root (im lazy)</li>
    <li>finish writing templating engine</li>
    <li>figure out how to efficiently handle events written in the templating engine and to trigger html changes (headache)</li>
    <li>create todolist application in chasm and add to readme</li>
    <li>publish <code>chasm</code> and become a billionaire</li>
</ul>
