
from django import forms


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