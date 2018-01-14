(function(){
    'use strict';

    var router = require('express').Router();

    router.use('/bucket', require('./bucket'));
    module.exports = router;
})();