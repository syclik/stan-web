var express = require('express');
var routes = require('./routes/index.js');
var api = require('./routes/api');
var http = require('http');
var path = require('path');

var app = module.exports = express();

/**
 * Configuration
 **/
app.set('port', 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.static(path.join(__dirname, 'public')));
app.use(app.router);

// Routes
app.get('/', routes.index);
app.get('/partial/:name', routes.partial);

// JSON API
app.get('/api/sample', api.sample);


http.createServer(app).listen(app.get('port'), function () {
  console.log('Express server listening on port ' + app.get('port'));
});
