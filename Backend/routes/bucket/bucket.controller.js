(function(){
    'use strict'; //prevents the use of faulty implementations in javascript

    var Bucket = require('./bucket.model.js'); //schema for bucket item

    var HardBuckets = [];
    // HardBuckets.push{id : HardBuckets.length + 1, realID : _id}

    module.exports.getById = function(req, res){
        Bucket.find({"_id": req.params.id}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            }return res.json(result);
        });
    };

    module.exports.getByLat = function(req, res){
        Bucket.find({"lat":{$eq: req.params.lat}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.getByName = function(req, res){
        Bucket.find({"name" : {$eq : req.params.name}}, function(err, results){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };


    module.exports.getByLong = function(req, res){
        Bucket.find({"long":{$eq: req.params.long}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.getAll = function(req, res){
        Bucket.find({}, function(err, buckets){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            }
            return res.json(buckets);
        });
    };

    module.exports.getByTemp = function(req, res){
        Bucket.find({"temp":{$eq: req.params.temp}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.getByVolume = function(req, res){
        Bucket.find({"volume":{$eq: req.params.volume}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };


    module.exports.getByIsFull = function(req, res){
        Bucket.find({"isFull": {$eq: req.params.isFull}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.setMaxVolume = function(req, res){
        Bucket.updateMany({}, {$set: {"maxVolume": req.params.allMax}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
        
    };

    module.exports.getByLocation = function(req, res){
        Bucket.find({"long" : {$eq: req.params.long}, "lat" : {$eq: req.params.lat}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            }return res.json(result);
        });
    };

    module.exports.add = function(req, res){
        if(!req.body.lat || !req.body.long || !req.body.volume || !req.body.temp){
            return res.status(400).send("Need a valid latitude, longitude, volume, and temeprature in the post body");
        }

        var bucket = new Bucket({
            name: req.body.name,
            lat: req.body.lat,
            long: req.body.long,
            temp: req.body.temp,
            volume: req.body.volume,
            maxVolume: req.body.maxVolume,
            isFull: false
        });

        bucket.save(function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } 
            HardBuckets.push({'id': HardBuckets.length, 'realID': req.body._id});
            console.log(HardBuckets.toString);
            // Add result._id as realID in HardBucket

            return res.json(result);
        });
    };

    module.exports.updateLocation = function(req, res){
        console.log()
        if (!req.body.id) {
            return res.status(400).send("No ID present");
        } else if ( typeof(req.body.lat) != 'number' || typeof(req.body.long) != 'number') {
            return res.status(400).send("Location not valid");
        }
        
        Bucket.update({'_id': req.body.id}, {$set: {'lat': req.body.lat, 'long': req.body.long}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });

    };

    module.exports.updateName = function(req, res){
        if(!req.body.id){
            return res.status(400).send("No ID present");
        } else if (typeof(req.body.name) != 'string'){
            return res.status(400).send("Name not valid type");
        }

        Bucket.update({'_id' : req.body.id}, {$set : {'name' : req.body.name}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.updateTemperature = function(req, res){
        if(!req.body.id){
            return res.status(400).send("No ID present");
        }else if (typeof(req.body.temp) != 'number'){
            return res.status(400).send("Temperature not valid");
        }
        Bucket.update({'_id': req.body.id}, {$set: {'temp': req.body.temp}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    module.exports.updateVolume = function(req, res){

        var realID;
        for (var i = 0; i < HardBuckets.length; i++){
            if(HardBuckets[i]._id == parseInt(req.body.id)){
                realID = HardBuckets[i].realID;
                break;
            }
        }
        // loop through hardbucket and get realID

        Bucket.update({'_id': realID}, {$set: {'volume': req.body.volume}}, function(err, result){
            if(err){
                console.log(err);
                return res.status(500).send(err);
            } return res.json(result);
        });
    };

    //Warning - the delete is not reversible; removes from database permanently. Proceed with own risk
    module.exports.delete = function(req, res){
        Bucket.findOneAndRemove({"_id": req.body.id}, function(err, result){
            if(err){
                console.log(err);
                return res.send(500).send(err);
            } return res.json(result);
        });
    };

    
})();