from django.shortcuts import render
from onlineapp.models import *
from django.views import View
from django import forms


class AddCollege(forms.ModelForm):
    class Meta:
        model = College
        exclude = ['id']
        widgets={
            'name': forms.TextInput(attrs={'class':'input','placeholder':'Enter Name'}),
            'location': forms.TextInput(attrs={'class':'input','placeholder':'Enter location'}),
            'acronym': forms.TextInput(attrs={'class':'input','placeholder':'enter acronym'}),
            'contact': forms.TextInput(attrs={'class':'input','placeholder':'enter contact'}),
        }

class AddStudent(forms.ModelForm):
    class Meta:
        model = Student
        exclude = ['id','college']
        widgets = {
            'name':forms.TextInput(attrs={'class':'input','placeholder':'Enter Name'}),
            'dob':forms.TextInput(attrs={'class':'input','placeholder':'Enter DOB'}),
            'email':forms.TextInput(attrs={'class':'input','placeholder':'Enter Email'}),
            #'college':forms.TextInput(attrs={'class':'input','placeholder':'Enter College'}),
        }

class AddMockTest1(forms.ModelForm):
    class Meta:
        model = MockTest1
        exclude = ['student','total']
        widgets = {
            'problem1' : forms.TextInput(attrs={'class':'input','placeholder':'Enter problem1'}),
            'problem2' : forms.TextInput(attrs={'class':'input','placeholder':'Enter problem2'}),
            'problem3' : forms.TextInput(attrs={'class':'input','placeholder':'Enter problem3'}),
            'problem4' : forms.TextInput(attrs={'class':'input','placeholder':'Enter problem4'}),
        }

class LoginForm(forms.Form):
    username = forms.CharField(
        max_length=50,
        required=True,
        widget= forms.TextInput(attrs={'class':'input','placeholder':'Enter Username'})
    )
    password = forms.CharField(
        max_length=50,
        required=True,
        widget=forms.PasswordInput(attrs={'class':'input','placeholder':'Enter Password'})
    )

class SignupForm(forms.Form):
    first_name=forms.CharField(
        max_length=40,
        required=True,
        widget=forms.TextInput(attrs={'class':'input','placeholder':'Enter First Name'})
    )
    last_name=forms.CharField(
        max_length=40,
        required=True,
        widget=forms.TextInput(attrs={'class':'input','placeholder':'Enter Last Name'})
    )
    username=forms.CharField(
        max_length=40,
        required=True,
        widget=forms.TextInput(attrs={'class':'input','placeholder':'Enter User Name'})
    )
    password=forms.CharField(
        max_length=40,
        required=True,
        widget=forms.PasswordInput(attrs={'class':'input','placeholder':'Enter password'})
    )
