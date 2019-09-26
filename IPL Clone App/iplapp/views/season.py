#from collections import OrderedDict
from django.core.paginator import Paginator
from django.shortcuts import render, redirect
from django.urls import resolve
from django.views import View

from iplapp.models import *


class SeasonView(View):
    def get(self,request,*args,**kwargs):
        all_seasons = Matches.objects.values('season').distinct()
        seasons_list = []
        for season in all_seasons:
            seasons_list.append(season['season'])
        seasons_list.sort(reverse=True)
        if kwargs:
            season = Matches.objects.filter(season=kwargs['year'])
        paginator = Paginator(season, 8) # Show 25 contacts per page

        page = request.GET.get('page')
        season = paginator.get_page(page)
        return render(
            request,
            template_name='season.html',
            context={'season': season,
                     'current_season': kwargs['year'],
                     'season_list': seasons_list},
        )


class SeasonPageView(View):
    def get(self,request,*args,**kwargs):
        all_seasons = Matches.objects.values('season').distinct()
        username = request.user.username
        seasons_list = []
        for season in all_seasons:
            seasons_list.append(season['season'])
        seasons_list.sort(reverse=True)
        if kwargs:
            season = Matches.objects.filter(season=kwargs['year'])
        start_position = (kwargs['page_no']-1)*8
        max_page = list(range(1, len(season)//8+2))
        if kwargs['page_no'] != 1:
            prev_page = kwargs['page_no'] - 1
        else:
            prev_page = kwargs['page_no']
        if kwargs['page_no'] != len(max_page):
            next_page = kwargs['page_no'] + 1
        else:
            next_page = kwargs['page_no']
        season = season[start_position:start_position+8]
        return render(
            request,
            template_name='season_page.html',
            context={'season': season,
                     'current_season': kwargs['year'],
                     'season_list': seasons_list,
                     'max_page': max_page,
                     'prev_page': prev_page,
                     'next_page': next_page,
                     'username': username},
        )


class MatchView(View):
    def get(self, request, *args, **kwargs):
        if not request.user.is_authenticated:
            return redirect('login')
        username = request.user.username
        match = Deliveries.objects.filter(match=kwargs['id'])
        season = Matches.objects.get(id=kwargs['id'])
        return render(
            request,
            template_name='match.html',
            context={'match': match,
                     'season': season,
                     'username': username}
        )

class PointsView(View):
    def get(self, request, *args, **kwargs):
        all_seasons = Matches.objects.values('season').distinct()
        username = request.user.username
        seasons_list = []
        for season in all_seasons:
            seasons_list.append(season['season'])
        seasons_list.sort(reverse=True)
        teams1 = Matches.objects.filter(season=kwargs['year'])
        teams2 = Matches.objects.filter(season=kwargs['year'])
        teams = []
        for team in teams1:
            teams.append(team.team1)
        for team in teams2:
            teams.append(team.team2)
        teams = list(set(teams))
        points = {}
        no_result_matches = Matches.objects.filter(result='no result', season=kwargs['year'])
        for match in no_result_matches:
            points[match.team1] = 0
            points[match.team2] = 0
        for team in teams:
            points[team] = Matches.objects.filter(winner=team, season=kwargs['year']).count()*2
        for match in no_result_matches:
            points[match.team1] = points[match.team1] + 1
            points[match.team2] = points[match.team2] + 1
        team_points = []
        for key, value in points.items():
            team_points.append([key, value])
        team_points.sort(key=lambda x: x[1], reverse=True)
        return render(
            request,
            template_name='points.html',
            context={'season_list': seasons_list,
                     'points': team_points,
                     'season': kwargs['year'],
                     'username': username}
        )


