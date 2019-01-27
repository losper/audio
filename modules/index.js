const imp=require("./impstream.js");

module.exports={
    InputStream:imp.InputStream,
    OutputStream:imp.OutputStream,
    fingerprint:imp.fingerprint,
    float32:1,
    int32:2,
    int24:4,
    int16:8,
    int8:16,
    uint8:32
}
