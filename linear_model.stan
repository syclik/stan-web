data {
  int N;
  vector<lower=-1, upper=1>[N] x;
  vector<lower=-1, upper=1>[N] y;
}
parameters {
  real alpha;
  real beta;
  real<lower=0> sigma_noise;
}
model {
  alpha ~ normal(0, 10);
  beta ~ normal(0, 10);
  sigma_noise ~ normal(0, 10);
  y ~ normal(alpha + x * beta, sigma_noise);
}
