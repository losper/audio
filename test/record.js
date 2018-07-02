var robot=require("./robot.js");
function workthread(rb){
    const opt={channels:2,format:8,rate:44100,frames:512,seconds:2};
    rb.record("test.pcm",10,opt);
    rb.record("test1.pcm",5,opt);
    rb.play("test.pcm");
    rb.play("test1.pcm");
}

new robot(workthread).start();
