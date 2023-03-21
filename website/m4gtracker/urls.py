#m4gtracker URL Configuration
from django.contrib 	import admin
from django.urls 	    import path, include
from main.views         import home, m0k1

urlpatterns = [
    # Landing
    path(''		            , home                                  ),
        
    # Framework
    path('accounts/'        , include('django.contrib.auth.urls')   ), # login, logout
    path('admin/'	        , admin.site.urls                       ),

    # M0-K1
    path('m0-k1/'           , m0k1                                  ),
    path('mO-k1/'           , m0k1                                  ),
    path('mo-k1/'           , m0k1                                  ),
    path('m0k1/'            , m0k1                                  ),
    path('mOk1/'            , m0k1                                  ),
    path('mok1/'            , m0k1                                  ),
    path('m0-ki/'           , m0k1                                  ),
    path('mO-ki/'           , m0k1                                  ),
    path('mo-ki/'           , m0k1                                  ),
    path('m0ki/'            , m0k1                                  ),
    path('mOki/'            , m0k1                                  ),
    path('moki/'            , m0k1                                  ),
    path('m0-kI/'           , m0k1                                  ),
    path('mO-kI/'           , m0k1                                  ),
    path('mo-kI/'           , m0k1                                  ),
    path('m0kI/'            , m0k1                                  ),
    path('mOkI/'            , m0k1                                  ),
    path('mokI/'            , m0k1                                  ),
    
    # Fallback
    path('<slug:slug>/'     , home                                  ),
]
 