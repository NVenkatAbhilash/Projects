
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from django.shortcuts import render, redirect
from django.views import View

from iplapp.forms.signupLogin import LoginForm, SignupForm


class LoginController(View):
    def get(self, request, *args, **kwargs):
        login = LoginForm()
        return render(
            request,
            template_name='loginSignup.html',
            context={'form':login,'title':'Login','link_name':'signup'}
        )
    def post(self,request,*args,**kwargs):
        loginForm = LoginForm(request.POST)
        if loginForm.is_valid():
            user = authenticate(
                request,
                username=loginForm.cleaned_data['username'],
                password=loginForm.cleaned_data['password']
            )
            if user is not None:
                login(request,  user)
                return redirect('season_page_url', year=2019, page_no=1)
        return render(
            request,
            template_name='loginSignup.html',
            context={'form':loginForm,'title':'Login','link_name':'signup','message':'invalid credentials'}
        )


class SignupController(View):
    def get(self,request,*args,**kwargs):
        form = SignupForm()
        return render(
            request,
            template_name='loginSignup.html',
            context={'form':form,'title':'Signin','link_name':'login'}
        )

    def post(self,request,*args,**kwargs):
        form = SignupForm(request.POST)
        if form.is_valid():
            user = User.objects.create_user(**form.cleaned_data)
            if user is not None:
                login(request,user)
                return redirect('season_page_url', year=2019,page_no=1)
        return render(
            request,
            template_name='loginSignup.html',
            context={'form':form,'title':'Signup','link_name':'login','message':'invalid credentials'}
        )

def logout_user(request):
    logout(request)
    return redirect('season_page_url', year=2019, page_no=1)