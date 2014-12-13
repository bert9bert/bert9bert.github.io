---
layout: post
title:  "A Comparison of Two Ways to Draw Beta-distributed Random Variables with Integer Parameters Starting with Uniform-distributed Random Variables"
date:   2014-12-14 12:00:00
categories: [math stat, C, simulation]
---

<span style="color:red">**WORK IN PROGRESS**</span>

# What are the two methods?

## Method 1: Using Order Statistics

Draw \\(\alpha+\beta-1\\) independent uniform random variables \\(U\_1, \dots, U\_{\alpha+\beta-1} \sim \mathrm{Uniform}(0,1) \\) i.i.d. and let \\(U\_{(k)}\\) be the \\(k^\text{th}\\) order statistic. Then,
\\[  U_{(\alpha)} \sim \mathrm{Beta}(\alpha,\beta)  \\]

Why is this true? The distribution function of the \\(k^\text{th}\\) uniform order statistic is

\\begin{aligned} F\_{(k)}(u) &= \mathbb{P}(U_{(k)} \leq u)\newline
&= \sum\_{j=k}^n \mathbb{P} \left( U\_{(1)} \leq u, \cdots,  U\_{(j)} \leq u, U\_{(j+1)} > u, \cdots,  U\_{(n)} > u \right)\newline
&= \sum\_{j=k}^n \dbinom{n}{j} u^j (1-u)^{n-j} \\end{aligned}

so the density function of the \\(k^\text{th}\\) uniform order statistic is

\\begin{aligned} f_{(k)}(u) &= \frac{d}{du} F\_{(k)}(u) = n \dbinom{n-1}{k-1} u^{k-1} (1-u)^{n-k} \newline
&= \frac{n!}{(k-1)!(n-k)!} u^{k-1} (1-u)^{n-k} \newline 
&= \frac{\Gamma(n+1)}{\Gamma(k)\Gamma(n-k+1)} u^{k-1} (1-u)^{n-k+1-1} \\end{aligned}
where the last part is because \\(\Gamma(n)=(n-1)!\\) when \\(n\\) is a positive integer. This is the density of a \\(\mathrm{Beta}(k, n-k+1)\\) distribution.

## Method 2: Using Gamma RVs

\\[ \frac{X}{X+Y} \sim \mathrm{Beta}(\alpha,\beta)  \\]
where \\(X \sim \mathrm{Gamma}(\alpha,1)\\) and \\(Y \sim \mathrm{Gamma}(\beta,1)\\) independent. Get gamma draws by drawing \\( U\_i \sim \mathrm{Uniform}(0,1) \\) i.i.d. for \\(i=1,\dots,c\\) and using
\\[ \sum_{i=1}^c -\log U_i \sim \mathrm{Gamma}(c,1)  \\]

Why is this true? A \\(\mathrm{Gamma}(1,1)\\) random variable is the same as an \\(\mathrm{Exp}(1)\\) random variable, which has the distribution function \\( F_{\mathrm{Exp}(1)}(x) = 1 - \mathrm{e}^{-x} \\) with support on \\(x \geq 0 \\), so the quantile function is \\( F\_{\mathrm{Exp}(1)}^{-1}(p) = - \log(1-p)  \\). Since this quantile function is strictly monotonic, if \\(U\\) is a standard Uniform RV, then \\(1-U\\) is also a standard Uniform RV, so \\(-\log(1-(1-U))=-\log(U)\\) is a  \\(\mathrm{Gamma}(1,1)\\) random variable. By the summation property of independent Gamma random variables, we get the above.

# What is the expected runtime?


# What is the simulated runtime?
