(function() {
    'use strict';

    var express = require('express');
    var controller = require('./bucket.controller.js');
    var computer = require('./bucket.compute.js');

    var router = express.Router();

    router.get('/:id', controller.getById);
    router.get('/lat/:lat', controller.getByLat);
    router.get('/long/:long', controller.getByLong);
    router.get('/temp/:temp', controller.getByTemp);
    router.get('/volume/:volume', controller.getByVolume);
    router.get('/isfull/:isFull', controller.getByIsFull);
    router.get('/getAll/getAll', controller.getAll);

   // router.get('/compute/totalMaple', computer.sumRandom);


    router.post('/', controller.add);
    router.post('/update/temp/', controller.updateTemperature);
    router.post('/update/volume/', controller.updateVolume);
    router.post('/update/location/', controller.updateLocation);
    router.post('/update/:allMax', controller.setMaxVolume);

    router.delete('/:id', controller.delete);

    module.exports = router;
})();