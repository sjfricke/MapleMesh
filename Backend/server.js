var express = require('express');
var bodyParser = require('body-parser');
var logger = require('morgan');

var app = express();
var api = require('./routes/api.js');

app.use(logger('dev'));

app.use(bodyParser.json()); //parses json and sets to body
app.use(bodyParser.urlencoded({ extended: false }));


var mongoose = require('mongoose');
var mongoURI = "mongodb://maplemesh:maplemesh18@ds155577.mlab.com:55577/maplemesh";
//mongoose.Promise = require('bluebird');
var mongodb = mongoose.connect(mongoURI, {useMongoClient: true});
mongodb.on('error', function(err){console.log(err.message);});
mongodb.once('open', function(){
    console.log("Connection established");
});

var ObjectID = mongodb.ObjectID;

app.use('/api', api);
// app.get('/test', function(req, res, next) {
//     res.json({"name": "test"});
// });
// app.post('/test', function(req, res, next) {
//     console.dir(req.body.name);
//     if (req.body.name.length < 2) {
//         return res.status(400).send("Need name of length greater than 2");
//     }
//     return res.status(200).send("yay");
// });
var port = process.env.PORT || 8080;
app.listen(port, function(){
    console.log("App is running on " + port);
});