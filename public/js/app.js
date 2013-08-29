'use strict';


// Declare app level module which depends on filters, and services
angular.module('StanWeb', ['StanWeb.filters', 'StanWeb.services', 'StanWeb.directives']).
  config(['$routeProvider', '$locationProvider', function($routeProvider, $locationProvider) {
    $routeProvider.when('/linear-model', 
                        {templateUrl: 'partial/linear-model',
                         controller: LinearModelCtrl}
                       );
    $routeProvider.otherwise({redirectTo: '/linear-model'});
    $locationProvider.html5Mode(true);
  }]);
