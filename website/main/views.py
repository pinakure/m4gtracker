from django.contrib.auth.decorators import login_required
from django.shortcuts import render,redirect,HttpResponse
from django.shortcuts import render
from django.contrib.staticfiles.utils   import get_files
from django.contrib.staticfiles.storage import StaticFilesStorage
import os

@staticmethod
def getSnapshots():
    res = []
    for path in os.listdir('static/snapshots'):
        if os.path.isfile(os.path.join('static/snapshots', path)):
            res.append(path)
    return res

@login_required()
def profile(request):
    #get user profile details
    #pass details to the view context
    context = [

    ]
    return render(request, 'profile.html')

def home(request, slug=None):
    if request.method == 'POST':
        if request.session.test_cookie_worked():
            request.session.delete_test_cookie()
            return HttpResponse(1)
        else:
            return HttpResponse(0)
    request.session.set_test_cookie()
    if slug=='profile':     return render(request, 'profile.html'       , context={ 'section' : 'User Profile'      } )
    if slug=='snapshots':   return render(request, 'snapshots.html'     , context={ 'section' : 'Snapshot Gallery'  , 'data' : getSnapshots() } )
    if slug=='bugtracker':  return render(request, 'bugtracker.html'    , context={ 'section' : 'BUGTracker'        } )
    if slug in('moki','mok1','m0ki','m0k1','mo-ki','mo-k1','m0-ki','m0-k1'): return m0k1(request)
    return render(request, 'home.html')

def m0k1(request):
    section = request.GET.get('section', None)
    if section == 'keys': return render(request, 'm0-k1-keys.html', context={'section': 'M0-K1 Keyboard Explanation' })
    return render(request, 'm0-k1.html', context={ 'section': 'M0-K1 Synthesizer'})
    