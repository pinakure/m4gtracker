#m4gtracker URL Configuration
from django.contrib 	import admin
from django.urls 	    import path, include
from main.views         import home

urlpatterns = [
    path(''		            , home                                  ),
    path('accounts/'        , include('django.contrib.auth.urls')   ), # login, logout
    path('admin/'	        , admin.site.urls                       ),
]
 