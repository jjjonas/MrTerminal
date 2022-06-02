
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import CurtainInformation from './CurtainInformation';
import CurtainStateRestForm from './CurtainStateRestForm';
import CurtainControl from './CurtainControl';

const CurtainProject: FC = () => {
  useLayoutTitle("Mr Curtain");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        {/* <Tab value="information" label="Information" /> */}
        {/* <Tab value="rest" label="REST" /> */}
        <Tab value="socket" label="Bedienung" />
      </RouterTabs>
      <Routes>
        {/* <Route path="information" element={<CurtainInformation />} /> */}
        {/* <Route path="rest" element={<CurtainStateRestForm />} /> */}
        <Route path="socket" element={<CurtainControl />} />
        <Route path="/*" element={<Navigate replace to="socket" />} />
      </Routes>
    </>
  );
};

export default CurtainProject;
