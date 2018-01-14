(function() {
    'use strict';

    var mongoose = require('mongoose');
    var Schema = mongoose.Schema;

    var BucketSchema = new Schema(
        {
            lat: Number,
            long: Number,
            temp: Number,
            volume: Number,
            maxVolume: Number,
            isFull: Boolean
        }, {"collection": "bucket"}
    );

    module.exports = mongoose.model('Bucket', BucketSchema);
})();