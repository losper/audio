var a2sync=require("a2sync");
var audio=require("audio");

module.exports=function(runable){
    var t;
    this.record=function(path,sec,opt){
	var x=new audio.InputStream(opt);
	x.on("data",function(data){
	    console.log(data);
	})
	x.on("done",function(){
	    console.log("record end!!!")
	    t.resume();
	    x.destroy();
	})
	x.record(sec);
	a2sync.yield();
    }
    this.play=function(){
	
    }
    this.start=function(){
	t=new a2sync(runable);
	t.resume(this);
    }
}
