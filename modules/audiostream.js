const Stream=require("stream");
const audio=require("./audio.passoa")
function InputStream(opt){
    InputStream.prototype.init.call(this);
    this.callback=function(ev,data){
	this.trigger(ev,data);
    }
    var inst=audio.open(opt.seconds,opt.channels,opt.format,opt.rate,opt.frames,this.callback.bind(this));
    this.resume=function(){
	audio.record(inst);
    }
    audio.record(inst);
}
InputStream.prototype = new Stream();

function OutputStream(opt){
    OutputStream.prototype.init.call(this);
    this.callback=function(){
	this.trigger.apply(this,arguments);
    }
    var inst=audio.open(opt.seconds,opt.channels,opt.format,opt.rate,opt.frames,this.callback.bind(this));
    
    this.on("data",function(data){
	console.log("push");
	audio.play(inst,data);
    });
    
}
OutputStream.prototype = new Stream();


module.exports={
    InputStream:InputStream,
    OutputStream:OutputStream,
    float32:1,
    int32:2,
    int24:4,
    int16:8,
    int8:16,
    uint8:32
}
