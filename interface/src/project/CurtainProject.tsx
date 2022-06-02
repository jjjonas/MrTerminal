
import React, { FC } from 'react';
import { Navigate, Route, Routes } from 'react-router-dom';

import { Tab } from '@mui/material';

import { RouterTabs, useRouterTab, useLayoutTitle } from '../components';

import CurtainInformation from './CurtainInformation';
import CurtainStateRestForm from './CurtainStateRestForm';
import CurtainStateWebSocketForm from './CurtainStateWebSocketForm';

const CurtainProject: FC = () => {
  useLayoutTitle("Mr Curtain");
  const { routerTab } = useRouterTab();

  return (
    <>
      <RouterTabs value={routerTab}>
        <Tab value="information" label="Information" />
        <Tab value="rest" label="REST" />
        <Tab value="socket" label="WebSocket" />
      </RouterTabs>
      <Routes>
        <Route path="information" element={<CurtainInformation />} />
        <Route path="rest" element={<CurtainStateRestForm />} />
        <Route path="socket" element={<CurtainStateWebSocketForm />} />
        <Route path="/*" element={<Navigate replace to="information" />} />
      </Routes>
    </>
  );
};

export default CurtainProject;
