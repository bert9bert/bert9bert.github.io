rm(list=ls())

library(ggplot2)

sims = read.fwf(file='beta_simulations_10000draws.txt',
	widths=c(5, 6, 11, 6, 6), header=TRUE, skip=2)
sims$method = factor(sims$method, 
	levels=c(1,2), labels=c('Order Statistics Method','Gamma RV Draws Method'))
sims$param_set = factor(sims$param_set, 
	levels=c(1,2), labels=c('one-sided','symmetric'))

p1 = ggplot(sims[sims$param_set=='one-sided',], 
	aes(x=beta, y=time, shape=method,color=method)) +
	ylab('Time to make 10,000 draws (sec)') + xlab(expression(beta)) +
	geom_point() +
	theme(legend.title=element_blank(), legend.position='bottom')
ggsave('fig_onesided.png')

p2 = ggplot(sims[sims$param_set=='symmetric',], 
	aes(x=beta, y=time, shape=method,color=method)) +
	ylab('Time to make 10,000 draws (sec)') + xlab(expression(beta)) +
	geom_point() +
	theme(legend.title=element_blank(), legend.position='bottom')
ggsave('fig_symmetric.png')

p3 = ggplot(sims[sims$param_set=='one-sided' & sims$beta<=50,], 
	aes(x=beta, y=time, shape=method,color=method)) +
	ylab('Time to make 10,000 draws (sec)') + xlab(expression(beta)) +
	geom_point() +
	theme(legend.title=element_blank(), legend.position='bottom')
ggsave('fig_onesided_low.png')

p4 = ggplot(sims[sims$param_set=='symmetric' & sims$beta<=25,], 
	aes(x=beta, y=time, shape=method,color=method)) +
	ylab('Time to make 10,000 draws (sec)') + xlab(expression(beta)) +
	geom_point() +
	theme(legend.title=element_blank(), legend.position='bottom')
ggsave('fig_symmetric_low.png')
