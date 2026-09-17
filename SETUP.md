# Setup GitHub Profile Metrics

File di folder `github-profile/` ini dipush ke repo khusus profil:
**https://github.com/antonprafanto/antonprafanto**

Repo itu sudah ada. `README.md`-nya akan tampil di halaman profil GitHub Anda.

---

## 1. Buat Personal Access Token (PAT)

1. Buka: https://github.com/settings/tokens
2. Pilih **Generate new token (classic)** (paling mudah untuk metrics)
3. Note: `metrics-profile`
4. Expiration: sesuai kebutuhan (mis. 90 days / No expiration)
5. Scope:
   - kosong saja sudah cukup untuk metrics dasar, **atau**
   - centang `public_repo` jika Action gagal menulis ke repo
6. Generate → **copy token** (hanya muncul sekali)

---

## 2. Tambah secret di repo profil

1. Buka: https://github.com/antonprafanto/antonprafanto/settings/secrets/actions
2. **New repository secret**
3. Name: `METRICS_TOKEN` (harus sama persis)
4. Value: paste token dari langkah 1
5. Save

---

## 3. Push file profil

Dari folder ini (`github-profile/`), di PowerShell:

```powershell
cd c:\Users\anton\vibecoding\github-profile

git init
git remote add origin https://github.com/antonprafanto/antonprafanto.git

# Ambil branch existing dulu (repo lama pakai master)
git fetch origin
git checkout -B master origin/master

# Salin isi baru (README + workflow). Jika file bentrok, overwrite dengan isi folder ini.
git add README.md .github
git status
git commit -m "Add metrics-style GitHub profile README and workflow"
git push -u origin master
```

Jika repo kosong / belum ada commit berguna, Anda juga bisa push langsung setelah `git init` + remote, tanpa fetch — tapi hati-hati menimpa isi lama.

---

## 4. Jalankan workflow Metrics

1. Buka: https://github.com/antonprafanto/antonprafanto/actions
2. Pilih workflow **Metrics**
3. **Run workflow** → Run
4. Tunggu sampai hijau (sukses)
5. Action akan commit file `github-metrics.svg` ke repo

---

## 5. Cek profil

Refresh: https://github.com/antonprafanto

Dashboard metrics muncul di bawah intro README.

---

## Opsional

### Private contributions di grafik / achievement

Settings → Public profile → centang **Include private contributions on my profile**

### PageSpeed rate limit

Kalau plugin PageSpeed sering error, buat API key gratis di
https://developers.google.com/speed/docs/insights/v5/get-started  
lalu tambah secret `PAGESPEED_TOKEN` dan di workflow:

```yaml
plugin_pagespeed_token: ${{ secrets.PAGESPEED_TOKEN }}
```

### Update otomatis

Workflow dijadwalkan tiap hari (`cron: 0 0 * * *`). Anda juga bisa menjalankan manual kapan saja dari tab Actions.

---

## Troubleshooting

| Masalah | Solusi |
| --- | --- |
| Workflow gagal auth | Pastikan secret bernama tepat `METRICS_TOKEN` |
| SVG tidak muncul | Pastikan path di README: `./github-metrics.svg` dan workflow sudah sukses commit |
| Permissions error saat commit | Di workflow sudah ada `permissions: contents: write`; pastikan PAT / default GITHUB_TOKEN boleh write |
| Topics kosong | Star beberapa [GitHub topics](https://github.com/topics) di akun Anda |
| Branch `main` vs `master` | Sesuaikan `git push` dan trigger `push.branches` dengan default branch repo |
