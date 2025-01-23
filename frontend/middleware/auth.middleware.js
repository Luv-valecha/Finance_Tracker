const jwt = require("jsonwebtoken");

const protectRoute= (req,res,next)=>{
    try {
        const token = req.cookies.jwt;

        if(!token){
            return res.status(401).json({ message: "Unauthorized - no token provided" });
        }

        const decoded= jwt.verify(token,process.env.JWT_SECRET)

        if(!decoded){
            return res.status(401).json({ message: "Unauthorized - invalid token" });
        }

        next();

    } catch (error) {
        console.log("Error in protectRoute middleware:", error.message);
        res.status(500).json({message: "Internal Server Error"});
    }
}

module.exports = { protectRoute };