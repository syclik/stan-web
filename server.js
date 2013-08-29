var express = require('express');
var api = require('./api');
var app = module.exports = express();

app.set('port', 3000);
app.get('/api/sample', api.sample);


app.get('/hello.txt', function(req, res){
  res.send('hello');
});

app.listen(3000);
console.log('Listening on port 3000');
