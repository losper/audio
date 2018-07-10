const Stream=require("stream");
const fs=require("fs");
function FileStream(){
    var rs;
    this.play=function(path){
	rs=new fs.ReadStream();
	rs.on("data",function(data){
	    if(this.trigger("data",data)){
		rs.pause();
	    }
	})

    }
    this.on("drain",function(){
	
    })
}
FileStream.prototype=new Stream();
