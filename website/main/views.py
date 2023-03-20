from django.contrib.auth.decorators import login_required
from django.shortcuts import render,redirect,HttpResponse
from django.shortcuts import render


@login_required()
def home(request):
	if request.method == 'POST':
		if request.session.test_cookie_worked():
			request.session.delete_test_cookie()
			return HttpResponse(1)
		else:
			return HttpResponse(0)
	request.session.set_test_cookie()
	return render(request, 'home.html')
    