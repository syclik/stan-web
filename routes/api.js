/*
 * Serve JSON to our AngularJS client
 */

var addon = require('../build/Release/linear_model');
var linear_model = new addon.LinearModel(100);

exports.sample = function(req, res) {
  res.json({
    sample: linear_model.sample()
  })
}
