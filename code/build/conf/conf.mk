#Configuration file for custom make options

#=========Change the correlator type used for the build =============
export COR=swc
#=========The file type can be set to either USB or file. USB reads data input from stdin while file reads from a #define'd file name ============
FRONTENDTYPE=FILE#USB/FILE
#========Set the data type being read into the code. I'd like to eventually change this to real/complex but for the moment it mimics the==========
#========SDR code with 1 being real (S1 S2 S3) and 2 being complex (I1 Q1 I2 Q2 I3 Q3)
FILETYPE=1
#======== Enable L2C processing on top of L1 processing  =================== 
#========= NOTE THIS IS IGNORED IF FRONTENDTYPE = USB as its impossible to feed two things into stdin right now =================
USEL2C=FALSE
