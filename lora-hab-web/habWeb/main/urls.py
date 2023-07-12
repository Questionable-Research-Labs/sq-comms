from django.urls import path
from .views import dashboard, api, map

urlpatterns = [
    path('dashboard/', dashboard, name='Dashboard'),
    path('api/', api, name="What you doin here?"),
    path("map/", map)
]