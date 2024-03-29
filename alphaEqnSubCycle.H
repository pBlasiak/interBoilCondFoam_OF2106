volScalarField divU(fvc::div(phi));

if (nAlphaSubCycles > 1)
{
    dimensionedScalar totalDeltaT = runTime.deltaT();
    surfaceScalarField rhoPhiSum
    (
        IOobject
        (
            "rhoPhiSum",
            runTime.timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("0", rhoPhi.dimensions(), 0)
    );

    tmp<volScalarField> trSubDeltaT;

    if (LTS)
    {
        trSubDeltaT =
            fv::localEulerDdt::localRSubDeltaT(mesh, nAlphaSubCycles);
    }

    for
    (
        subCycle<volScalarField> alphaSubCycle(alpha1, nAlphaSubCycles);
        !(++alphaSubCycle).end();
    )
    {
        #include "alphaEqn.H"
        rhoPhiSum += (runTime.deltaT()/totalDeltaT)*rhoPhi;
    }

    rhoPhi = rhoPhiSum;
}
else
{
    #include "alphaEqn.H"
}

// from interThermalPhaseChangeFoam
//mixture->interfaceProperties::correct();

//rho = 1.0/
//	  (
//	  	(1.0/rho1 - 1.0/rho2)*alpha1
//	   + 1.0/rho2
//	  );
//rhoCp = 1.0/
//	  (
//	  	(1.0/(rho1*cp1) - 1.0/(rho2*cp2))*alpha1
//	   + 1.0/(rho2*cp2)
//	  );

//rho == alpha1*rho1 + alpha2*rho2;
//rhoCp == alpha1*rho1*cp1 + alpha2*rho2*cp2;
