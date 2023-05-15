from django.shortcuts import render
import datetime

# Create your views here.
from django.http import HttpResponse

# ******** firebase part
# from firebase import firebase
import json
# # firebase = firebase.FirebaseApplication('https://nodemcu2-a6ae3-default-rtdb.asia-southeast1.firebasedatabase.app', None)
# firebase = firebase.FirebaseApplication('https://sem-prjct-esp32-temp-hum-default-rtdb.asia-southeast1.firebasedatabase.app', None)

import pyrebase

#Initialize Firebase
firebaseConfig = {
  "apiKey": "AIzaSyCLAxE2c2WufWLmy-gxPtUSctlVo9uOnC0",
  "authDomain": "sem-prjct-esp32-temp-hum.firebaseapp.com",
  "databaseURL": "https://sem-prjct-esp32-temp-hum-default-rtdb.asia-southeast1.firebasedatabase.app",
  "projectId": "sem-prjct-esp32-temp-hum",
  "storageBucket": "sem-prjct-esp32-temp-hum.appspot.com",
  "messagingSenderId": "409828985893",
  "appId": "1:409828985893:web:f36247d46beaea258ec3cc"
}
# firebaseConfig=json.dumps(firebaseConfig)
# print(firebaseConfig)

firebase=pyrebase.initialize_app(firebaseConfig)

db=firebase.database()



### ******** Main intake temp_hum
Intake_tm=None
Intake_temp=None
Intake_hum=None

Intake_all_rcrd=[]

### ******** return
rtn_tm=None
rtn_temp=None
rtn_hum=None
rtn_all_rcrd=[]


def index(request):
    # print(request)
    return HttpResponse("Hello, world. You're at the polls index.")

def index1(request):
    return HttpResponse("Hel1, world. You're at the polls index.")

def intake(request):
    global Intake_tm
    global Intake_temp
    global Intake_hum
    tmp_0=str(request)
    tmp_1=tmp_0.find("?")
    tmp_dta=tmp_0[tmp_1:]   #?temp=30-hum=70
    tmp_3=tmp_dta.find("-")
    tmp_dta_temp=tmp_dta[:tmp_3]  #temp=30
    tmp_dta_hum=tmp_dta[tmp_3:]   #hum=70
    tmp_euqal_temp=tmp_dta_temp.find("=")  
    fin_temp=tmp_dta_temp[tmp_euqal_temp+1:]   # 30
    tmp_euqal_hum=tmp_dta_temp.find("=")
    fin_hum=tmp_dta_hum[tmp_euqal_hum:-2]    #70

    intake_tmp_tm = datetime.datetime.now()   # 5-5-2023 20:00hr
    # print(incm_veh,",",incm_veh_tm)
    # unallowed_rfid_lst.append(object)
    # return HttpResponse("Rfid_data_here")
    # vh_crnt_pos="at_rfid"
    Intake_tm=intake_tmp_tm
    Intake_temp=fin_temp
    Intake_hum=fin_hum
    # print("intake",Intake_tm,Intake_temp,Intake_hum)
    Intake_all_rcrd.append([Intake_tm,Intake_temp,Intake_hum])
    
    # send firebase data for rfid   # work here
    # tmp_frs_addr='/UsersData/fms/entries/2'   # make this n to new id 
    # tmp_frbs_dt={'vehicle_id': incm_veh,}
    # fr_snd_dta(tmp_frs_addr,tmp_frbs_dt)
    # return HttpResponse("Veh_rfid_noted move ahead")

    #****** for humidity
    tmp_frs_addr='/s1_intake/hum'   # make this n to new id 
    # tmp_frbs_dt={'time': Intake_tm,'humidity':Intake_hum,'temp':Intake_temp}
    # tmp_frbs_dt={"1":{Intake_tm.strftime("%m/%d/%Y, %H:%M:%S"):Intake_hum,}}
    tmp_str_time=Intake_tm.strftime("%d-%m-%Y, %H:%M:%S")
    # tmp_frbs_dt={Intake_tm.strftime("%m/%d/%Y, %H:%M:%S"):Intake_hum,}
    # frb_intake_sent = json.dumps(tmp_frbs_dt)
    # print(frb_intake_sent)
    fr_snd_dta(tmp_frs_addr,tmp_str_time,Intake_hum)
    # tmp_frbs_dt={'name':50,'url': 20}
    # fr_snd_dta(tmp_frs_addr,tmp_frbs_dt)

    #****** for temp
    tmp_frs_addr='/s1_intake/temp'   # make this n to new id 
    # tmp_frbs_dt={'time': Intake_tm,'humidity':Intake_hum,'temp':Intake_temp}
    # tmp_frbs_dt={"1":{Intake_tm.strftime("%m/%d/%Y, %H:%M:%S"):Intake_hum,}}
    # tmp_frbs_dt={Intake_tm.strftime("%m/%d/%Y, %H:%M:%S"):Intake_temp,}
    # # frb_intake_sent = json.dumps(tmp_frbs_dt)
    # print(frb_intake_sent)
    fr_snd_dta(tmp_frs_addr,tmp_str_time,Intake_temp)
    return HttpResponse("Intake_sens_data_added")

def rtrn(request):
    global rtn_tm
    global rtn_temp
    global rtn_hum
    tmp_0=str(request)
    tmp_1=tmp_0.find("?")
    tmp_dta=tmp_0[tmp_1:]   #?temp=30-hum=70
    tmp_3=tmp_dta.find("-")
    tmp_dta_temp=tmp_dta[:tmp_3]  #temp=30
    tmp_dta_hum=tmp_dta[tmp_3:]   #hum=70
    tmp_euqal_temp=tmp_dta_temp.find("=")  
    fin_temp=tmp_dta_temp[tmp_euqal_temp+1:]   # 30
    tmp_euqal_hum=tmp_dta_temp.find("=")
    fin_hum=tmp_dta_hum[tmp_euqal_hum:-2]    #70

    rtn_tmp_tm = datetime.datetime.now()   # 5-5-2023 20:00hr
    rtn_tm=rtn_tmp_tm
    rtn_temp=fin_temp
    rtn_hum=fin_hum
    print("Retrn",Intake_tm,Intake_temp,Intake_hum)

    rtn_all_rcrd.append([rtn_tm,rtn_temp,rtn_hum])
    
    # send firebase data for rfid   # work here
    tmp_frs_addr='/s2_return/hum'   # make this n to new id 
    # tmp_frbs_dt={'time': Intake_tm,'humidity':Intake_hum,'temp':Intake_temp}
    # tmp_frbs_dt={rtn_tm.strftime("%m//%d//%Y, %H:%M:%S"):rtn_hum,}
    tmp_str_time=rtn_tm.strftime("%d-%m-%Y, %H:%M:%S")
    # print(tmp_frbs_dt)
    fr_snd_dta(tmp_frs_addr,tmp_str_time,rtn_hum)

    tmp_frs_addr='/s2_return/temp'   # make this n to new id 
    # tmp_frbs_dt={'time': Intake_tm,'humidity':Intake_hum,'temp':Intake_temp}
    # tmp_frbs_dt={rtn_tm.strftime("%m//%d//%Y, %H:%M:%S"):rtn_temp,}
    # print(tmp_frbs_dt)
    fr_snd_dta(tmp_frs_addr,tmp_str_time,rtn_temp)
    return HttpResponse("Return_sens_data_added")
    pass

## ********* debugging code zone

def chckng_cd_wrkng(request):
    global incm_veh
    global vh_crnt_pos
    global vh_id_pos_n_tm
    dt_to_disply=fomtng_disply()
    # print(incm_veh)
    # print(dt_to_disply)
    return HttpResponse(dt_to_disply)
    pass

def fomtng_disply():
    global incm_veh
    global vh_crnt_pos
    global vh_id_pos_n_tm
    temp=""
    temp=temp+"veh_currnt_pos:-"+str(vh_crnt_pos)+"<br>"
    # print("hello")
    # print(vh_id_pos_n_tm)
    # try:
    for i in vh_id_pos_n_tm:
        try:
            temp+=i[0]+","
        except:
            temp+="None"+","
        temp+=i[1].strftime("%m/%d/%Y, %H:%M:%S")+","
        try:
            temp+=i[2]+","
        except:
            temp+="None"+","
        temp+="<br>"
    # except:
    #     pass
    return temp
    pass


def fr_snd_dta(addrs,tm,data):
    # result = firebase.post(addrs,json.dumps(data))
    # result = firebase.patch("/s2_return/temp/-NQRK4MPxH_ysbWlKadz.json/",json.dumps(data))
    result = db.child(addrs).child(tm).set(data)
    print("Data send to firebase:-",result)
    pass


'''
rough here


incmin_rfid
http://192.168.137.1:8000/hndl_url/rfid/1?013E7826


'''