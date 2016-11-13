int matinv(double **a,int n)
{
    double **inv,tmp;
    int i,j,k,retval;

    retval = 0;
    inv = new double *[n];
    for (i=0;i<n;i++) {
        inv[i] = new double [n];
    }
// Initialize identity matrix
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            inv[i][j] = 0.0;
        }
        inv[i][i] = 1.0;
    }

    for (k=0;k<n;k++) {
        tmp = a[k][k];
        if (tmp == 0) {
            retval = 1;
            goto _100;
        }
        for (j=0;j<n;j++) {
            if (j>k) a[k][j] /= tmp;    // Don't bother with previous entries
     
       inv[k][j] /= tmp;
        }
        for (i=0;i<n;i++) {             // Loop over rows
            if (i == k) continue;
            tmp = a[i][k];
            for (j=0;j<n;j++) {
                if (j>k) a[i][j] -= a[k][j]*tmp;
                inv[i][j] -= inv[k][j]*tmp;
            }
        }
    }

// Copy inverse to source matrix
    for (i=0;i<n;i++) {
        for (j=0;j<n;j++) {
            a[i][j] = inv[i][j];
        }
    }
_100:
    for (i=0;i<n;i++) {
        delete [] inv[i];
    }
    delete [] inv;
    return retval;
}
