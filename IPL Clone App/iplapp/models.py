from django.db import models

# Create your models here.


class Matches(models.Model):
    id = models.IntegerField(primary_key=True)
    season = models.IntegerField()
    city = models.CharField(max_length=40,blank=True, null=True)
    date = models.DateField()
    team1 = models.CharField(max_length=40)
    team2 = models.CharField(max_length=40)
    toss_winner = models.CharField(max_length=40)
    toss_decision = models.CharField(max_length=10)
    result = models.CharField(max_length=10)
    dl_applied = models.IntegerField()
    winner = models.CharField(max_length=40, blank=True, null=True)
    win_by_runs = models.IntegerField()
    win_by_wickets = models.IntegerField()
    player_of_match = models.CharField(max_length=40, blank=True, null=True)
    venue = models.CharField(max_length=60)
    umpire1 = models.CharField(max_length=40, blank=True, null=True)
    umpire2 = models.CharField(max_length=40, blank=True, null=True)
    umpire3 = models.CharField(max_length=40, blank=True, null=True)

    def __int__(self):
        return self.match_id


class Deliveries(models.Model):
    match = models.ForeignKey(Matches, on_delete=models.CASCADE)
    inning = models.IntegerField()
    batting_team = models.CharField(max_length=40)
    bowling_team = models.CharField(max_length=40)
    over = models.IntegerField()
    ball = models.IntegerField()
    batsman = models.CharField(max_length=40)
    non_striker = models.CharField(max_length=40)
    bowler = models.CharField(max_length=40)
    is_super_over = models.IntegerField()
    wide_runs = models.IntegerField()
    bye_runs = models.IntegerField()
    legbye_runs = models.IntegerField()
    noball_runs = models.IntegerField()
    penalty_runs = models.IntegerField()
    batsman_runs = models.IntegerField()
    extra_runs = models.IntegerField()
    total_runs = models.IntegerField()
    player_dismissed = models.CharField(max_length=40, blank=True, null=True)
    dismissal_kind = models.CharField(max_length=40, blank=True, null=True)
    fielder = models.CharField(max_length=40, blank=True, null=True)

    def __int__(self):
        return self.match_id

class DuplicateDeliveries(models.Model):
    match = models.ForeignKey(Matches, on_delete=models.CASCADE)
    inning = models.IntegerField()
    batting_team = models.CharField(max_length=40)
    bowling_team = models.CharField(max_length=40)
    over = models.IntegerField()
    ball = models.IntegerField()
    batsman = models.CharField(max_length=40)
    non_striker = models.CharField(max_length=40)
    bowler = models.CharField(max_length=40)
    is_super_over = models.IntegerField()
    wide_runs = models.IntegerField()
    bye_runs = models.IntegerField()
    legbye_runs = models.IntegerField()
    noball_runs = models.IntegerField()
    penalty_runs = models.IntegerField()
    batsman_runs = models.IntegerField()
    extra_runs = models.IntegerField()
    total_runs = models.IntegerField()
    player_dismissed = models.CharField(max_length=40, blank=True, null=True)
    dismissal_kind = models.CharField(max_length=40, blank=True, null=True)
    fielder = models.CharField(max_length=40, blank=True, null=True)

    def __int__(self):
        return self.match_id
