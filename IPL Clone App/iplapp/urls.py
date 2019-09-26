from django.conf import settings
from django.conf.urls import include,url
from django.contrib import admin
from django.shortcuts import redirect
from django.urls import path, include
import debug_toolbar
from rest_framework.authtoken import views

from iplapp.views.season import *
from iplapp.views.signupLogin import *

urlpatterns = [
    #path('', lambda x: redirect('season_page_url', year=2019, page_no=1)),
    path('seasons/<int:year>/', SeasonView.as_view(), name='season_url'),
    path('seasons/<int:year>/<int:page_no>/', SeasonPageView.as_view(), name='season_page_url'),
    path('seasons/<int:year>/match/<int:id>/', MatchView.as_view(), name='match_url'),
    path('seasons/<int:year>/points/', PointsView.as_view(), name='points_url'),
    path('login/', LoginController.as_view(),name = 'login'),
    path('signup/', SignupController.as_view(),name = 'signup'),
    path('logout/', logout_user, name='logout'),
]