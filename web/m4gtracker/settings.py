from pathlib import Path
import os
BASE_DIR                    = Path(__file__).resolve().parent.parent
SECRET_KEY                  = os.environ.get('DJANGO_SECRET')
SITE_NAME                   = os.environ.get('SITE_NAME')
DEBUG                       = True
SITE_ID                     = 1
URL_ROOT                    = os.environ.get('SITE_HOST')
ALLOWED_HOSTS               = [ URL_ROOT ]
ROOT_URLCONF                = f'{ SITE_NAME }.urls'
TIME_ZONE                   = 'UTC'
USE_I18N                    = True
USE_TZ                      = True
LANGUAGE_CODE               = 'en-us'
STATIC_URL                  = 'static/'
STATIC_ROOT                 = f'/static/{ SITE_NAME }/'
MEDIA_ROOT                  = f'/media/{ SITE_NAME }/'
DEFAULT_AUTO_FIELD          = 'django.db.models.BigAutoField'
WSGI_APPLICATION            = f'{ SITE_NAME }.wsgi.application'
USE_X_FORWARDED_HOST        = True
CORS_ALLOW_ALL_ORIGINS      = True
SECURE_PROXY_SSL_HEADER     = ('HTTP_X_FORWARDED_PROTO', 'https')
SOCIALACCOUNT_LOGIN_ON_GET  = True
CSRF_TRUSTED_ORIGINS        = [ f'https://{ URL_ROOT}', 'https://cpanel.iskarion.ddns.net' ]
LOGIN_REDIRECT_URL          = "/"
LOGOUT_REDIRECT_URL         = "/"
LOGIN_URL                   = '/accounts/oidc/keycloak/login/'
X_FRAME_OPTIONS             = 'ALLOW'
INSTALLED_APPS              = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'django.contrib.sites',                             # KeyCloak
    'allauth',                                          # KeyCloak
    'allauth.account',                                  # KeyCloak
    'allauth.socialaccount',                            # KeyCloak
    'allauth.socialaccount.providers.openid_connect',   # KeyCloak
    'corsheaders',                                      # Cross Domain
]
MIDDLEWARE                  = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    "corsheaders.middleware.CorsMiddleware",
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',
    'allauth.account.middleware.AccountMiddleware',# Keycloak
]
TEMPLATES                   = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': ['templates'],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]
DATABASE_POSTGRES           = {
    'ENGINE'    : 'django.db.backends.postgresql',
    'NAME'      : os.environ.get('DB_NAME'),
    'USER'      : os.environ.get('DB_USER'),
    'PASSWORD'  : os.environ.get('DB_PASSWORD'),
    'HOST'      : os.environ.get('DB_HOST'),
    'PORT'      : '5432',
} 
DATABASE_SQLITE3            = {
    'ENGINE'    : 'django.db.backends.sqlite3',
    'NAME'      : BASE_DIR / 'db.sqlite3',
} 
DATABASES                   = { 'default': DATABASE_POSTGRES, }
AUTH_PASSWORD_VALIDATORS    = [
    {'NAME': 'django.contrib.auth.password_validation.UserAttributeSimilarityValidator',},
    {'NAME': 'django.contrib.auth.password_validation.MinimumLengthValidator',},
    {'NAME': 'django.contrib.auth.password_validation.CommonPasswordValidator',},
    {'NAME': 'django.contrib.auth.password_validation.NumericPasswordValidator',},
]
SOCIALACCOUNT_PROVIDERS     = {
    "openid_connect": {
        "APPS": [
            {
                "provider_id"   : "keycloak",
                "name"          : "Keycloak",
                "client_id"     : SITE_NAME,
                "secret"        : os.environ.get('KC_SECRET'),
                "settings"      : {
                    "server_url": "https://auth.iskarion.ddns.net/realms/Iskarion/.well-known/openid-configuration",
                },
            }
        ],
        "AUTH_PARAMS": {
            "prompt": "login"
        }
    },
}
AUTHENTICATION_BACKENDS = [
    'django.contrib.auth.backends.ModelBackend',# Necesario para iniciar sesión con usuarios de Django nativos (ej. admin)
    'allauth.account.auth_backends.AuthenticationBackend',# Necesario para los métodos de autenticación específicos de django-allauth (Keycloak)
]