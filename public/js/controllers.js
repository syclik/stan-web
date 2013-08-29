'use strict';

/* Controllers */

function AppCtrl($scope, $http) {
  /*$http({method: 'GET', url: '/api/name'}).
    success(function(data, status, headers, config) {
    $scope.name = data.name;
    }).
    error(function(data, status, headers, config) {
    $scope.name = 'Error!'
    });*/
}

function LinearModelCtrl($scope, $http) {
  $http({method: 'GET', url: '/api/sample'})
    .success(function(data, status, headers, config) {
      $scope.data = data;
    })
    .error(function(data, status, headers, config) {
      $scope.data = 'ERROR';
    });
}


function MyCtrl1() {}
MyCtrl1.$inject = [];


function MyCtrl2() {
}
MyCtrl2.$inject = [];
