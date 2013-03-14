data {
  int N;
  vector<lower=-1, upper=1>[N] x;
  vector<lower=-1, upper=1>[N] y;
}
parameters {
  real<lower=0> sigma_noise;
  real alpha;
  real beta;
}
model {
  y ~ normal(alpha + x * beta, sigma_noise);
}
