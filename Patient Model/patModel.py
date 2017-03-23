# -*- coding: utf-8 -*-
"""
Patient Model built from https://physionet.org/challenge/2012/#weight

@author: Theo Fountain III
"""
import glob, os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import matplotlib.dates as mdates
import matplotlib
import pickle
import math, datetime
import time


from sklearn import preprocessing, cross_validation, svm
from sklearn.linear_model import LinearRegression

style.use('ggplot')

def get_VS(file):
##Patient vital signs
    file = open(file)
    df = pd.read_csv(file)

##Patient outcomes 
    outcome = open('Outcomes-a.txt')
    dfout = pd.read_csv(outcome)

## Extract patient info from Patient Record header

    table = df.loc[lambda df:df.Time == '00:00']
    table.is_copy = False
    table.drop("Time",axis = 1,inplace = True)
    table.set_index('Parameter',inplace = True)
    table = table['Value'] 
    table.index = ['RecordID', 'Age', 'Gender', 'Height(cm)', 'ICUType', 'Weight(kg)']
    table.RecordID.astype(int)
   
##Check patient survival using outcomes
    patOut = dfout.loc[lambda dfout:dfout.RecordID == table['RecordID']]
    patID = str(int(patOut['RecordID']))
    live = int(patOut['Survival'])
    stay = int(patOut['Length_of_stay'])

    status = 'Patient '
    if (live == -1) :
        status = status+'survived'
    elif (live >= 2) or (live<= stay):
        status = status+'died in Hospital'

## Data cleansing/preprocessing 
    df = df[5:]

## Time -> Timedelta
    df["Time"] = df["Time"]+":00"
    t = pd.to_timedelta(df["Time"])

## Timedelta -> Datetime
    t = t+pd.Timestamp('20130101')
    df['Time'] = t
    df.index = t
    
## Extract vital signs and merge with timestamps
    fi = df.loc[lambda df:df.Parameter == "FiO2"]
    fi.is_copy = False
    fi.drop("Parameter",axis = 1,inplace = True)
    fi.columns = ['Time','FiO2']
    fi["FiO2"] = 100*fi["FiO2"]

    sa = df.loc[lambda df:df.Parameter == "SaO2"]
    sa.is_copy = False
    sa.drop("Parameter",axis = 1,inplace = True)
    sa.columns = ['Time','SaO2']

    pa = df.loc[lambda df:df.Parameter == "PaO2"]
    pa.is_copy = False
    pa.drop("Parameter",axis = 1,inplace = True)
    pa.columns = ['Time','PaO2']

    pac = df.loc[lambda df:df.Parameter == "PaCO2"]
    pac.is_copy = False
    pac.drop("Parameter",axis = 1,inplace = True)
    pac.columns = ['Time','PaCO2']

    hr = df.loc[lambda df:df.Parameter == "HR"]
    hr.is_copy = False
    hr.drop("Parameter",axis = 1,inplace = True)
    hr.columns = ['Time','HR']

    new = pd.merge(fi,sa,how = 'outer')
    new = pd.merge(new,pa,how = 'outer')
    new = pd.merge(new,pac,how = 'outer')
    new = pd.merge(new,hr,how = 'outer')
    pat = pd.merge(df,new, how = 'outer')

    pat.drop("Parameter",axis = 1,inplace = True)
    pat.drop("Value",axis = 1,inplace = True)
    pat.drop_duplicates('Time',keep ='first',inplace = True)

    pat.index = pat.Time
    pat = pat.resample('30Min').mean()

## Missing Data
##    pat.dropna(inplace=True)
##    pat.fillna(method = 'ffill', inplace = True)
##    pat.fillna(method = 'bfill', inplace = True)
  
# Hierarchical Index using RecordID
    iterables = [[patID],[u'FiO2',u'SaO2',u'PaO2',u'PaCO2',u'HR']]
    index = pd.MultiIndex.from_product(iterables,names=['RecordID:','VS:'])
    pat.columns = index
    
    return pat,table,patID

## Plot respiratory vital signs

def plot_VS(icu,patID):

    # Make new DataFrames for plotting 
    resp = pd.DataFrame(icu[icu.columns[0:2]])
    press = pd.DataFrame(icu[icu.columns[2:4]])

    
    plt.figure()
    
    ax1 = plt.subplot2grid((4,1),(0,0),rowspan=2,colspan=1)
    prplt = resp.plot(ax=ax1,legend=False,ylim=[0,100],linewidth=2,label='Respiratory')
    ax1.yaxis.set_major_locator(mticker.MaxNLocator(nbins=5,prune='lower'))
    plt.title('Patient: '+patID)
    plt.ylabel('%')
    ax1.legend()
    leg1 = ax1.legend(loc=2,ncol=1,prop={'size':11})
    leg1.get_frame().set_alpha(0.4)

    axh = ax1.twinx()
    axh.grid(False)
    hplt = icu[icu.columns[4]].plot(ax=axh,legend=False,ylim=[0,100],color='c',linewidth=2,label='Heart Rate')
    axh.yaxis.set_major_locator(mticker.MaxNLocator(nbins=5,prune='lower'))
    plt.ylabel('BPM')
    axh.legend()
    legh = axh.legend(loc=1,ncol=1,prop={'size':11})
    legh.get_frame().set_alpha(0.4)

    ax2 = plt.subplot2grid((4,1),(2,0),rowspan=2,colspan=1)
    prplt = press.plot(ax=ax2,legend=False,ylim=[0,450],linewidth=2,rot=45,label='Blood Pressure')
    ax2.yaxis.set_major_locator(mticker.MaxNLocator(nbins=5,prune='upper'))
    plt.ylabel('mmHg')
    ax2.legend()
    leg2 = ax2.legend(loc=2,ncol=1,prop={'size':11})
    leg2.get_frame().set_alpha(0.4)
    
    plt.subplots_adjust(left=0.11,bottom=0.24,right=0.9,top=0.9,wspace=0.2,hspace=0)
    plt.show()

    return 
    
    
def fetchData(data):

    os.chdir(data)

    # Patient Outcomes 
    sigs = []
    patient = []
    recID = []
    num = 0
    for file in glob.glob("*.txt"):
        while num <10:
            
            if file != 'Outcomes-a.txt':
                    p = get_VS(file)
                    
                    sigs.append(p[0])
                    patient.append(p[1])
                    recID.append(p[2])
                    
                    
#    p_frame = sigs.to_frame('Patient0')
#    num = 1
#    for num in range(len(Patient_info[1:])):
#        patient[num].name = 'Patient' +str(num)
#        p_frame[patient[num].name] = patient[num]


    pickle_out1 = open('ICU_data.pickle','wb')
    pickle.dump(sigs, pickle_out1)
    pickle_out1.close()

#    pickle_out2 = open('Patient_info.pickle','wb')
#    pickle.dump(p_frame, pickle_out2)
#    pickle_out2.close()
    
    pickle_out3 = open('Patient_frame.pickle','wb')
    pickle.dump(patient, pickle_out3)
    pickle_out3.close()

    return

# Determine if Patient survived or died in hospital TODO: integrate with get_VS)

##Survival > Length of stay  ⇒  Survivor
##Survival = -1  ⇒  Survivor
##2 ≤ Survival ≤ Length of stay  ⇒  In-hospital death

##RecordID (a unique integer for each ICU stay)
##Age (years)
##Gender (0: female, or 1: male)
##Height (cm)
##ICUType (1: Coronary Care Unit, 2: Cardiac Surgery Recovery Unit, 3: Medical ICU, or 4: Surgical ICU)
##Weight (kg)


def patSurv(pat,outcome):
    survived =  dfout.Survival
    stay = dfout.Length_of_stay
    
    alive = np.equal(survived,-1)
    
    d1 = np.greater_equal(survived, 2)
    d2 = np.greater_equal(survived, stay)
    dead = np.logical_or(d1,d2)
    
    livcheck = np.logical_not(dead[:])
    liv = np.logical_or(alive,livcheck)
    
    #Survived
    d = {1:True,0:False}
    liv = liv.map(d)
    liv.astype(bool,inplace = True)
    l = liv.tolist()
    l = [l.count(True),l.count(False)]
    
    # Died in hospital 
    hdeath = dfout['In-hospital_death']
    hdeath = hdeath.map(d)
    hdeath.astype(bool,inplace = True)
    h = hdeath.tolist()
    h = [h.count(True),h.count(False)]
    
    d = {'Survived':l,'Hospital Death':h}
    pdf = pd.DataFrame(d,index = ['True','False'])
    pdf.plot(title = 'Patient Outcomes',kind='bar');
    
    dur = dfout['Length_of_stay'].mean()
    stay_range = stay.max()-stay.min()


#Patient Stats TODO: Stats dataframe
# rolling and static

def stat_VS(icu):

    icu_mean = icu.mean()
    icu_range = icu.max()-icu.min()
    icu_norm = (icu - icu_mean)/icu_range
    ICU_corr = icu_norm.corr()
    
    df_std = df.rolling(window=4,center=False).std()
    plot_VS(df_std,p[2]+'_STD')
    
    df_avg = df.rolling(window=4,center=False).mean()
    plot_VS(df_avg,p[2]+'_AVG')
    
    #Drop stagnant FiO2 for corr (too little change)
    df_Fi = df.loc[:'2013-01-01 05:00:00']
    df_corr = df_Fi['FiO2'].rolling(window=10,center=False).corr(df_Fi['SaO2'])
    df_corr.plot()
    plt.show()
    
    #Drop PaO2 outliers???
    df['PaO2_STD'] = pd.rolling_std(df['PaO2'],10)
    df_std = df.describe()['PaO2']['std']
    print(df_std)
    df = df[(df['PaO2_STD'] < df_std)]
    df.drop('PaO2_STD',axis=1,inplace = True)

    
#ICU_data[0].columns = index
    
def info_Out():
    pfram = Patient_info[0].to_frame('Patient0')
    icu_frame = ICU_data[0]
    outcome = open('Outcomes-a.txt')
    dfout = pd.read_csv(outcome)
    
    for num in range(1000):
        Patient_info[num].name = 'Patient' +str(num)
        pfram[Patient_info[num].name] = Patient_info[num]
        
    pfram = pfram.T
    pfram = pfram.astype(int)
    pfram.replace(-1,np.nan,inplace = True)
    
    pfram.set_index('RecordID',drop = True,inplace = True)
    pfram.columns = ['Age(yrs)', 'Gender', 'Height(cm)', 'ICUType', 'Weight(kg)']
    
    mf = pfram['Gender'].astype(bool).tolist()
    mf = [mf.count(True),mf.count(False)]
    
    g = {'Male':mf[0],'Female':mf[1]}
    mf1 = pd.DataFrame(g,index = ['Count'])
    
    g_ratio = round(mf[0]/mf[1],2)
    mf1.plot(kind = 'bar', legend = True,title='Gender Ratio:'+str(g_ratio),color = ['r','g'],alpha=0.5,figsize=(6,4),use_index=False)
    
    df1 = pd.DataFrame({'Age(yrs)':pfram['Age(yrs)'],
                        'Height(cm)':pfram['Height(cm)'],
                        'Weight(kg)':pfram['Weight(kg)']},columns = ['Age(yrs)','Height(cm)','Weight(kg)'])
    plt.figure
    df1.plot.hist(alpha=0.25,stacked = True,bins = 10,figsize=(6,4),title='Patient Demograpics')


## Regression Forecasting
def fcast(icu):
    df = p[0]
    
    forecast_col =  'HR'
    
    df.fillna(value=-99999, inplace=True)
    forecast_out = int(math.ceil(0.3* len(df)))
    df['label'] = df[forecast_col].shift(-forecast_out)
    
    X = np.array(df.drop(['label'], 1))
    X = preprocessing.scale(X)
    X_lately = X[-forecast_out:]
    X = X[:-forecast_out]
    
    df.dropna(inplace=True)
    
    y = np.array(df['label'])
    
    X_train, X_test, y_train, y_test = cross_validation.train_test_split(X, y, test_size=0.2)
    clf = LinearRegression(n_jobs=-1)
    #clf = svm.SVR()
    #for k in ['linear','poly','rbf','sigmoid']:
    #    clf = svm.SVR(kernel=k)
    #    clf.fit(X_train,y_train)
    #    confidence = clf.score(X_test,y_test)
    #    print(k,confidence)
    
    
    clf.fit(X_train, y_train)
    confidence = clf.score(X_test, y_test)
    
    
    forecast_set = clf.predict(X_lately)
    df['Forecast'] = np.nan
    
    last_date = df.iloc[-1].name
    last_unix = last_date.timestamp()
    thirty_min = 1800
    next_unix = last_unix + thirty_min
    
    for i in forecast_set:
        next_date = datetime.datetime.fromtimestamp(next_unix)
        next_unix += thirty_min
        df.loc[next_date] = [np.nan for _ in range(len(df.columns)-1)]+[i]
    
    print(confidence)
    
    df[forecast_col].plot(ylim = [0,100])
    df['Forecast'].plot()
    plt.legend(loc=4)
    plt.title('Vital Sign forecasting')
    plt.show()


#Cluster
def pat_cluster(pfram):
    dfout[pfram.columns]=pfram[:]
    dfout.dropna(axis = 0, how='any',inplace=True)
    die = dfout['In-hospital_death'].astype(bool)
    dfout['In-hospital_death'] = die
    
    df = dfout    
    
    X = np.array(df.drop(['In-hospital_death'],1).astype(float))
    X.preprocessing.scale(X)
    y = np.array(df['In-hospital_death'])
    
    #clf = KMeans(n_clusters=2)
    clf.fit(X)
    
    correct = 0
    
    for i in range(len(X)):
          predict_me = np.array(X[i].astype(float))
          predict_me = predict_me.reshape(-1,len(predict_me))
          prediction = clf.predict(predict_me)
          if prediction[0] == y[i]:
            (correct/len(X))
## ANN

# Load patient signals and info

pickle_in1 = open('ICU_data.pickle','rb')
ICU_data = pickle.load(pickle_in1)

pickle_in2 = open('Patient_info.pickle','rb')
Patient_info = pickle.load(pickle_in2)

#pickle_in3 = open('Patient_frame.pickle','rb')
#p_frame = pickle.load(pickle_in3)

##### Patient Outcomes 
####outcome = open('Outcomes-a.txt')
####dfout = pd.read_csv(outcome)
##
###Patient record
##pat = Patient_info[0]
##pat.index = ['RecordID', 'Age', 'Gender', 'Height(cm)', 'ICUType', 'Weight(kg)']




