var express = require('express');
var app = express();
var addon = require('./build/Release/linear_model');
var linear_model = new addon.LinearModel(100);


app.get('/hello.txt', function(req, res){
  res.send(linear_model.sample());
});

app.listen(3000);
console.log('Listening on port 3000');
