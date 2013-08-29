'use strict';

// client side angular js code
angular.module('linear-model', [])
  .config(['$routeProvider', '$locationProvider', function($routeProvider, $locationProvider) {
    $routeProvider.when('/linear-model', {templateUrl: 'partial/linear-model', controller: LinearModelCtrl});
    $routeProvider.otherwise({redirectTo: '/linear-model'});
    $locationProvider.html5Mode(true);
  }]);
