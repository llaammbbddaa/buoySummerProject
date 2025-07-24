## FORMULA - incredibly basic
prob = 100 * (1 - (percentDiffpH + percentDifftemp + percentDifftds));



# **OUTDATED**

## formula
Accuracy: 0.7460545193687231
Intercept: 0.970945804309344
Coefficients: $[-0.12291875 -0.02812187 -0.00668885]$
pH          : -0.1229
Salinity    : -0.0281
Water Temp  : -0.0067


## explanation
basically we needed a way to be able to enter in our sensor data values and get back some analysis as to the likelihood of an algal bloom occurring
so we developped a logistic regression model to determine that for us
to develop it, we took a large data set from the gulf of mexico, and surrounding areas of florida
and then trimmed the data to suit our needs, (only the sensors / data that are relevant), and added a boolean column showing whether or not an algal bloom had historically occurred
this is likely not the most accurate model, but for our purposes, it will do just fine
