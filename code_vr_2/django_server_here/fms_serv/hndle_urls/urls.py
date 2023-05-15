from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('1', views.index1, name='index1'),

    ## rfid------  http://192.168.137.1:8000/hndl_url/rfid?013E7826
    # --- http://localhost:8000/hndl_url/intake?temp=30-hum=70
    path('intake', views.intake, name='intake'),
    # path('1', views.index1, name='index1'),

    # --- http://localhost:8000/hndl_url/rtrn?temp=30-hum=70
    path('rtrn', views.rtrn, name='rtrn'),
    # path('1', views.index1, name='index1'),
    
    
    ### others debugging------- http://192.168.137.1:8000/hndl_url/chckng_cd_wrkng
    # --- http://localhost:8000/hndl_url/chckng_cd_wrkng
    path('chckng_cd_wrkng', views.chckng_cd_wrkng, name='chckng_cd_wrkng'),    

]