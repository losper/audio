const imp=require("./impstream.js");
function record(path,sec,opt){
    var ins=new imp.InputStream(opt);
    ins.on("data",function(data){
	    
    });
    ins.on("done",function(){
	    
    });
    ins.record(sec);

}

module.exports={
    InputStream:imp.InputStream,
    OutputStream:imp.OutputStream,
    float32:1,
    int32:2,
    int24:4,
    int16:8,
    int8:16,
    uint8:32
}
