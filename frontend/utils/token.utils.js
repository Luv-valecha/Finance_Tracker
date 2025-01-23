const jwt = require("jsonwebtoken");

// Generates a JSON Web Token
const generateToken = (username, res) => {

    const token = jwt.sign({ username }, process.env.JWT_SECRET, { expiresIn: "3d" });

    res.cookie("jwt", token, {
        maxAge: 3 * 24 * 60 * 60 * 1000, // 3 days in milliseconds
        httpOnly: true, // Prevents client-side JavaScript from accessing the cookie
        sameSite: true, // Prevents cross-site request forgery (CSRF) attacks
        secure: process.env.NODE_ENV !== "development", // Sends the cookie only over HTTPS in production
    });
    return token;
};

module.exports = { generateToken };