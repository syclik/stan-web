data {
  int N;
  vector<lower=-1, upper=1>[N] x;
  vector<lower=-1, upper=1>[N] y;
}
parameters {
  real<lower=-1,upper=1> alpha;
  real<lower=-10,upper=10> beta;
  real<lower=0> sigma_noise;
}
model {
  //alpha ~ normal(0, 10);
  //beta ~ normal(0, 10);
  sigma_noise ~ normal(0, 1);
  y ~ normal(alpha + x * beta, sigma_noise);
}
